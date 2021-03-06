package iforest;

import java.util.Random;

public class ITree {

    public int attrIndex;
    public double attrValue;
    public int leafNodes;
    public int curHeight;
    public ITree lTree, rTree;

    //initializing ITree node
    public ITree(int p, double pval) {

        this.curHeight = 0;
        this.leafNodes = 1;
        this.attrIndex = p;
        this.attrValue = pval;
        this.lTree = null;
        this.rTree = null;
    }

    public static ITree createITree(double[][] samples, int curHeight, int maxHeight) {
        // tree initialization
        ITree iTree = null;
        if (samples.length == 0) {
            return iTree;
        } 
        
        else if (curHeight >= maxHeight || samples.length == 1) {
            iTree = new ITree(0, samples[0][0]);
            iTree.leafNodes = 1;
            iTree.curHeight = curHeight;
            return iTree;
        }

        int rows = samples.length;
        int cols = samples[0].length;
        boolean isAllSame = true;
        break_label:
        for (int i = 0; i < rows - 1; i++) {
            for (int j = 0; j < cols; j++) {
                if (samples[i][j] != samples[i + 1][j]) {
                    isAllSame = false;
                    break break_label;
                }
            }
        }

        if (isAllSame == true) {
            iTree = new ITree(0, samples[0][0]);
            iTree.leafNodes = samples.length;
            iTree.curHeight = curHeight;
            return iTree;
        }
        //Generating random
        Random random = new Random(System.currentTimeMillis());
        int attrIndex = random.nextInt(cols);
        double min, max;
        min = samples[0][attrIndex];
        max = min;
        for (int i = 1; i < rows; i++) {
            if (samples[i][attrIndex] < min) {
                min = samples[i][attrIndex];
            }
            if (samples[i][attrIndex] > max) {
                max = samples[i][attrIndex];
            }
        }

        double attrValue = random.nextDouble() * (max - min) + min;
        int lnodes = 0, rnodes = 0;
        double curValue;
        for (int i = 0; i < rows; i++) {
            curValue = samples[i][attrIndex];
            if (curValue < attrValue) {
                lnodes++;
            } else {
                rnodes++;
            }
        }

        double[][] lSamples = new double[lnodes][cols];
        double[][] rSamples = new double[rnodes][cols];

        lnodes = 0;
        rnodes = 0;
        for (int i = 0; i < rows; i++) {
            curValue = samples[i][attrIndex];
            if (curValue < attrValue) {
                lSamples[lnodes++] = samples[i];
            } else {
                rSamples[rnodes++] = samples[i];
            }
        }

        ITree root = new ITree(attrIndex, attrValue);
        root.leafNodes = rows;
        root.curHeight = curHeight;
        root.lTree = createITree(lSamples, curHeight + 1, maxHeight);
        root.rTree = createITree(rSamples, curHeight + 1, maxHeight);

        return root;
    }
}
