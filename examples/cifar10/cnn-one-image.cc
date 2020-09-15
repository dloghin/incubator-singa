#include "cnn-one-image.h"

namespace singa {
// currently supports 'cudnn' and 'singacpp'
#ifdef USE_CUDNN
const std::string engine = "cudnn";
#else
const std::string engine = "singacpp";
#endif  // USE_CUDNN

static const size_t kImageDim = 32;
static const size_t kImageSize = 32 * 32 * 3 + 1;

LayerConf GenConvConf(string name, int nb_filter, int kernel, int stride,
		int pad, float std) {
	LayerConf conf;
	conf.set_name(name);
	conf.set_type(engine + "_convolution");
	ConvolutionConf *conv = conf.mutable_convolution_conf();
	conv->set_num_output(nb_filter);
	conv->add_kernel_size(kernel);
	conv->add_stride(stride);
	conv->add_pad(pad);
	conv->set_bias_term(true);

	ParamSpec *wspec = conf.add_param();
	wspec->set_name(name + "_weight");
	auto wfill = wspec->mutable_filler();
	wfill->set_type("Gaussian");
	wfill->set_std(std);

	ParamSpec *bspec = conf.add_param();
	bspec->set_name(name + "_bias");
	bspec->set_lr_mult(2);
	//  bspec->set_decay_mult(0);
	return conf;
}

LayerConf GenPoolingConf(string name, bool max_pool, int kernel, int stride,
		int pad) {
	LayerConf conf;
	conf.set_name(name);
	conf.set_type(engine + "_pooling");
	PoolingConf *pool = conf.mutable_pooling_conf();
	pool->set_kernel_size(kernel);
	pool->set_stride(stride);
	pool->set_pad(pad);
	if (!max_pool) pool->set_pool(PoolingConf_PoolMethod_AVE);
	return conf;
}

LayerConf GenReLUConf(string name) {
	LayerConf conf;
	conf.set_name(name);
	conf.set_type(engine + "_relu");
	return conf;
}

LayerConf GenDenseConf(string name, int num_output, float std, float wd) {
	LayerConf conf;
	conf.set_name(name);
	conf.set_type("singa_dense");
	DenseConf *dense = conf.mutable_dense_conf();
	dense->set_num_output(num_output);

	ParamSpec *wspec = conf.add_param();
	wspec->set_name(name + "_weight");
	wspec->set_decay_mult(wd);
	auto wfill = wspec->mutable_filler();
	wfill->set_type("Gaussian");
	wfill->set_std(std);

	ParamSpec *bspec = conf.add_param();
	bspec->set_name(name + "_bias");
	bspec->set_lr_mult(2);
	bspec->set_decay_mult(0);

	return conf;
}

LayerConf GenLRNConf(string name) {
	LayerConf conf;
	conf.set_name(name);
	conf.set_type(engine + "_lrn");
	LRNConf *lrn = conf.mutable_lrn_conf();
	lrn->set_local_size(3);
	lrn->set_alpha(5e-05);
	lrn->set_beta(0.75);
	return conf;
}

LayerConf GenFlattenConf(string name) {
	LayerConf conf;
	conf.set_name(name);
	conf.set_type("singa_flatten");
	return conf;
}

FeedForwardNet CreateNet() {
	FeedForwardNet net;
	Shape s{3, 32, 32};

	net.Add(GenConvConf("conv1", 32, 5, 1, 2, 0.0001), &s);
	net.Add(GenPoolingConf("pool1", true, 3, 2, 1));
	net.Add(GenReLUConf("relu1"));
	net.Add(GenLRNConf("lrn1"));
	net.Add(GenConvConf("conv2", 32, 5, 1, 2, 0.01));
	net.Add(GenReLUConf("relu2"));
	net.Add(GenPoolingConf("pool2", false, 3, 2, 1));
	net.Add(GenLRNConf("lrn2"));
	net.Add(GenConvConf("conv3", 64, 5, 1, 2, 0.01));
	net.Add(GenReLUConf("relu3"));
	net.Add(GenPoolingConf("pool3", false, 3, 2, 1));
	net.Add(GenFlattenConf("flat"));
	net.Add(GenDenseConf("ip", 10, 0.01, 250));
	return net;
}

const std::pair<Tensor, Tensor> ReadImageFile(string file) {
	Tensor image(Shape{1, 3, kImageDim, kImageDim});
	Tensor label(Shape{1}, kInt);
	std::ifstream data_file(file.c_str(), std::ios::in | std::ios::binary);
	if (!data_file.is_open())
		LOG(ERROR) << "Unable to open file " << file;
	char buff[kImageSize];
	float data[kImageSize-1];
	data_file.read(buff, kImageSize);
	int label_val = (int)buff[0];
	label.CopyDataFromHostPtr(&label_val, 1, 0);
	for (int i = 0; i < kImageSize - 1; i++)
		data[i] = (float)((int)buff[i+1]);
	// image.CopyDataFromHostPtr(data, sizeof(float) * (kImageSize - 1), 0);
	image.CopyDataFromHostPtr(data, kImageSize - 1, 0);
	data_file.close();
	return std::make_pair(image, label);
}

int Eval(string file) {

	Tensor test_x, test_y;
	auto test = ReadImageFile(file);
	/*
    size_t nsamples = test.first.shape(0);
    auto mtest = Reshape(test.first, Shape{nsamples, test.first.Size() / nsamples});
    const Tensor& mean = Average(mtest, 0);
    SubRow(mean, &mtest);
    test_x = Reshape(mtest, test.first.shape());
	 */
	test_x = test.first;
	test_y = test.second;

	CHECK_EQ(test_x.shape(0), test_y.shape(0));
	LOG(INFO) << "Test samples = " << test_y.shape(0);
	auto net = CreateNet();
	SGD sgd;
	OptimizerConf opt_conf;
	opt_conf.set_momentum(0.9);
	auto reg = opt_conf.mutable_regularizer();
	reg->set_coefficient(0.004);
	sgd.Setup(opt_conf);
	sgd.SetLearningRateGenerator([](int step) {
		if (step <= 120)
			return 0.001;
		else if (step <= 130)
			return 0.0001;
		else
			return 0.00001;
	});

	SoftmaxCrossEntropy loss;
	Accuracy acc;
	net.Compile(true, &sgd, &loss, &acc);

#ifdef MY_FILE_READER
	FileReader snap;
	snap.OpenForRead("myfilesnap.bin");
#else
	Snapshot snap(snapshot_name, Snapshot::kRead, 100);
#endif
	vector<std::pair<std::string, Tensor>> params = snap.Read();
	net.SetParamValues(params);

#ifdef USE_CUDNN
	auto dev = std::make_shared<CudaGPU>();
	net.ToDevice(dev);
	test_x.ToDevice(dev);
	test_y.ToDevice(dev);
	Tensor ttmp =  net.EvaluateOnBatchOutput(test_x, test_y);
	Tensor tout = ttmp.ToHost();
#else
	Tensor tout =  net.EvaluateOnBatchOutput(test_x, test_y);
#endif  // USE_CUDNN

	float vals[10];
	tout.GetValue(vals, 10);
	float max = vals[0];
	int max_pos = 0;
	LOG(INFO) << 0 << " " << vals[0];
	for (int i = 1; i < 10; i++) {
		if (max < vals[i]) {
			max = vals[i];
			max_pos = i;
		}
		LOG(INFO) << i << " " << vals[i];
	}
#ifdef MY_FILE_READER
	snap.Close();
#endif
	return max_pos;
}
}

#ifdef LOCAL_MAIN
int main(int argc, char **argv) {
	singa::InitChannel(nullptr);

	if (argc < 2) {
		LOG(INFO) << "Usage: " << argv[0] << " <image_bin>" << std::endl;
		return -1;
	}

	//  LOG(INFO) << "Start evaluation";
	int idx = singa::Eval(argv[1]);
	LOG(INFO) << "Label: " << idx;
	//  LOG(INFO) << "End evaluation";
}
#endif // LOCAL_MAIN
