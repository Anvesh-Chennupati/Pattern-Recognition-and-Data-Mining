package iforest;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class IForest {
    //used while computing the anomaly anomalyscores
    private Double c0;
    private Double c1;
    private int subSampleSize;

    //creating a list of ITrees
    private List<ITree> iTreeList;

    public IForest() {
        this.c0 = null;
        this.c1 = null;
        // as per paper subsamplesize is set to 256
        this.subSampleSize = 256;
        this.iTreeList = new ArrayList<>();
    }

    public double[] build(double[][] samples, int t) throws Exception {
        return buildForest(samples, t, 256,100);
    }

    public double[] buildForest(double[][] samples, int t, int subSampleSize, int iters) throws Exception {
        //Checking if subsample size is greater than sample size and resizing it
        this.subSampleSize = subSampleSize;
        if (this.subSampleSize > samples.length) {
            this.subSampleSize = samples.length;
        }

        createIForest(samples, t);

        double[] anomalyscores = computeAnomalyIndex(samples);
        double[] labels = classifyscores(anomalyscores, iters);
        return labels;
    }

    private double[] classifyscores(double[] anomalyscores, int iters) {

        c0 = anomalyscores[0];
        c1 = anomalyscores[0];

        for (int i = 1; i < anomalyscores.length; i++) {
            if (anomalyscores[i] > c0) {
                c0 = anomalyscores[i];
            }

            if (anomalyscores[i] < c1) {
                c1 = anomalyscores[i];
            }
        }

        int cnt0, cnt1; double diff0, diff1;
        double[] ascore = new double[anomalyscores.length];

        for (int n = 0; n < iters; n++) {
            cnt0 = 0;
            cnt1 = 0;

            for (int i = 0; i < anomalyscores.length; i++) {
                diff0 = Math.abs(anomalyscores[i] - c0);
                diff1 = Math.abs(anomalyscores[i] - c1);

                ascore[i] = anomalyscores[i];

                if (diff0 < diff1) {
                    cnt0++;
                } else {
                    cnt1++;
                }
            }
            diff0 = c0; diff1 = c1;

            c0 = 0.0; c1 = 0.0;
            for (int i = 0; i < anomalyscores.length; i++) {
                if (ascore[i] == 0) {
                    c0 += anomalyscores[i];
                } else {
                    c1 += anomalyscores[i];
                }
            }
            c0 /= cnt0; c1 /= cnt1;

            // terminating condition
            if (c0 - diff0 <= -100000 && c1 - diff1 <= -100000) {
                break;
            }
        }
        return ascore;
    }

    private void createIForest(double[][] samples, int t) throws Exception {

        // max heigt of the tree;
        int limitHeight = (int) Math.ceil(Math.log(subSampleSize) / Math.log(2));
        // creating iTree object
        ITree iTree;
        double[][] subSample;

        for (int i = 0; i < t; i++) {
            subSample = this.getSubSamples(samples, subSampleSize);
            iTree = ITree.createITree(subSample, 0, limitHeight);
            this.iTreeList.add(iTree);
        }
    }

    private double[][] getSubSamples(double[][] samples, int sampleNum) throws Exception {

        if (samples == null || samples.length == 0) {
            throw new Exception("Samples is Invalid");
        } else if (sampleNum <= 0) {
            throw new Exception("sub sample less than 0");
        }
        if (samples.length < sampleNum) {
            sampleNum = samples.length;
        }
        int cols = samples[0].length;
        double[][] subSamples = new double[sampleNum][cols];

        int randomIndex;
        Random random = new Random(System.currentTimeMillis());
        for (int i = 0; i < sampleNum; i++) {
            randomIndex = random.nextInt(samples.length);
            subSamples[i] = samples[randomIndex];
        }

        return subSamples;
    }
    private double[] computeAnomalyIndex(double[][] samples) throws Exception {

        if (samples == null || samples.length == 0) {
            throw new Exception("Invalid sample size");
        }

        double[] anomalyscores = new double[samples.length];
        for (int i = 0; i < samples.length; i++) {
            //computing the anomaly of individual sample 
            anomalyscores[i] = computeAnomalyIndex(samples[i]);
        }

        return anomalyscores;
    }

    private double computePathLength(double[] sample, final ITree iTree) throws Exception {

        if (sample == null || sample.length == 0) {
            throw new Exception("Sample is empty");
        } else if (iTree == null || iTree.leafNodes == 0) {
            throw new Exception("Tree is empty");
        }

        double pathLength = -1; double attrValue;
        ITree tmpITree = iTree;

        while (tmpITree != null) {
            pathLength += 1;
            attrValue = sample[tmpITree.attrIndex];

            if (tmpITree.lTree == null || tmpITree.rTree == null || attrValue == tmpITree.attrValue) {
                break;
            } else if (attrValue < tmpITree.attrValue) {
                tmpITree = tmpITree.lTree;
            } else {
                tmpITree = tmpITree.rTree;
            }
        }

        return pathLength + computeCn(tmpITree.leafNodes);
    }

    private double computeCn(double n) {
        if (n <= 1) {
            return 0;
        }
        return 2 * (Math.log(n - 1) + 0.5772156649) - 2 * ((n - 1) / n);
    }
    
    private double computeAnomalyIndex(double[] sample) throws Exception {

        if (iTreeList == null || iTreeList.size() == 0) {
            throw new Exception("Invalid. Tree is empty. Create forest first");
        } else if (sample == null || sample.length == 0) {
            throw new Exception("Sample is empty");
        }

        double ehx = 0;
        double pathLength = 0;
        for (ITree iTree : iTreeList) {
            pathLength = computePathLength(sample, iTree);
            ehx += pathLength;
        }
        ehx /= iTreeList.size();
        double cn = computeCn(subSampleSize);
        double index = ehx / cn;
        double anomalyIndex = Math.pow(2, -index);
        return anomalyIndex;
    }
}
