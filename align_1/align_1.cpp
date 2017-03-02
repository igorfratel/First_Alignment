#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#define GAP -2

/*Similarity matrix. eg: Position [0][1] contains the similarity between 1 and 2.*/
int **sim_matrix;
/*Similarity matrix dimension (dim x dim)*/
int dim;
/*Dynamic programming matrix*/
int **dyn_matrix;

void createSimMatrix(std::ifstream &fsim_matrix) {
    /*Receives the address to an ifstream (a .txt file).
     *Reads the first word of the file, which is the matrix dimension.
     *Fills the global variable matrix "sim_matrix" based on the numbers 
     *contained in the file.*/
    std::string str_dim;
    fsim_matrix.open("similarity_matrix.txt");
    if (fsim_matrix) {
        fsim_matrix >> str_dim;
        dim = std::stoi(str_dim);
        sim_matrix = new int *[dim];
        for (int i = 0; i < dim; i++)
            sim_matrix[i] = new int[dim];
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
             fsim_matrix >> sim_matrix[i][j];
        fsim_matrix.close();
    }
}

void printSimMatrix() {
    /*Prints the global variale "sim_matrix" to the standard output.*/
    std::cout << "sim_matrix dimensions: "<< dim << "x" << dim;
    for (int i = 0; i < dim; i++) {
        std::cout << "\n";
        for (int j = 0; j < dim; j++)
            std::cout << sim_matrix[i][j] << " ";
    }
    std::cout << "\n";
}

void deleteSimMatrix() {
    /*Frees space allocated for the global variable "sim_matrix"*/
    for (int i = 0; i < dim ; i++)
        delete [] sim_matrix[i];
    delete [] sim_matrix;
}

void createSeqArray(std::ifstream &fseq, std::vector<int> &seq,
                    std::string filename) {
    /*Receives the address to an ifstream (a .txt file), a reference to an int 
     *vector and a string.
     *Fills seq with the elements from the file.*/
    std::string str_len;
    int value;
    fseq.open(filename);
    if (fseq) {
        while (fseq >> value)
            seq.push_back(value);
        fseq.close();
    }
}

void printSeqs(std::vector<int> const &seq_1, std::vector<int> const &seq_2) {
    /*Receives two sequence vectors and prints them to the standard output.*/
    std::cout << "Sequences:\n";
    for (std::vector<int>::size_type i = 0; i < seq_1.size(); i++)
        std::cout << seq_1[i] << " ";
    std::cout << "\n";
    for (std::vector<int>::size_type i = 0; i < seq_2.size(); i++)
        std::cout << seq_2[i] << " ";
    std::cout << "\n";
}

void createDynMatrix(int m, int n) {
    /*Receives the ints m and n and allocates a mxn 2d array in the global
     *variable dyn_matrix.*/
    dyn_matrix = new int *[m];
    for (int i = 0; i < m; i++)
        dyn_matrix[i] = new int[n];
}

void deleteDynMatrix(int m, int n) {
    /*Frees the allocated memory for the global 2d mxn array dyn_matrix.*/
    for (int i = 0; i < m; i++)
        delete [] dyn_matrix[i];
    delete [] dyn_matrix;
}

void printDynMatrix (int m, int n) {
    /*Prints the mxn global 2d array dyn_matrix.*/
    std::cout << "dyn_matrix dimensions: "<< m << "x" << n;
    for (int i = 0; i < m; i++) { 
        std::cout << "\n";
        for (int j = 0; j < n; j++)
            std::cout << dyn_matrix[i][j] << " ";
    }
    std::cout << "\n";
}

int computeSim (std::vector<int> const &seq_1,  std::vector<int> const &seq_2) {
    /*Receives two sequences (int vectors).
     *Computes and returns the global similarity between the sequences using the
     *gap penalty (#define GAP) and the sim_matrix.*/
    int match;
    int options[3];
    for (std::vector<int>::size_type i = 0; i <= seq_1.size(); i++)
        dyn_matrix[i][0] = i * GAP;
    for (std::vector<int>::size_type i = 0; i <= seq_2.size(); i++)
        dyn_matrix[0][i] = i * GAP;
    for (std::vector<int>::size_type i = 1; i <= seq_1.size(); i++)
        for (std::vector<int>::size_type j = 1; j <= seq_2.size(); j++) {
            match = sim_matrix[seq_1[i-1]-1][seq_2[j-1]-1];
            options[0] = dyn_matrix[i-1][j] + GAP;
            options[1] = dyn_matrix[i-1][j-1] + match;
            options[2] = dyn_matrix[i][j-1] + GAP;
            dyn_matrix[i][j] = *std::max_element(options, options + 3);
        }
    return dyn_matrix[seq_1.size()][seq_2.size()];
}

void align(int i, int j, int len, std::vector<std::string> &align_1, 
           std::vector<std::string> &align_2, std::vector<int> const &seq_1, 
           std::vector<int> const &seq_2) {
    /*Receives two integers (the sizes of the first and second sequences), 
     *an int len = 0, two string vectors where the aligned sequences will be 
     *stored and the two sequences to be aligned (int vectors).
     *Using the dyn_matrix, recovers the upmost optimal alignment between the 
     *two sequences.*/
    if (i == 0 && j == 0)
        len = 0;
    else if (i > 0 && dyn_matrix[i][j] == dyn_matrix[i-1][j] + GAP) {
        align(i - 1, j, len, align_1, align_2, seq_1, seq_2);
        len++;
        align_1.push_back(std::to_string(seq_1[i-1]));
        align_2.push_back("-");
    }
    else if (i > 0 && j > 0 && dyn_matrix[i][j] == dyn_matrix[i-1][j-1] + 
             sim_matrix[seq_1[i-1]-1][seq_2[j-1]-1]) {
        align(i - 1, j - 1, len, align_1, align_2, seq_1, seq_2);
        len++;
        align_1.push_back(std::to_string(seq_1[i-1]));
        align_2.push_back(std::to_string(seq_2[j-1]));
    }
    else {
        align(i, j - 1, len, align_1, align_2, seq_1, seq_2);
        len++;
        align_1.push_back("-");
        align_2.push_back(std::to_string(seq_2[j-1]));
    }
}

void printAlignment(const std::vector<std::string> &align_1, 
                    const std::vector<std::string> &align_2) {
    /*Receives the two aligned sequences (string vectors) and prints them to the
     *standard output.*/
    std::cout << "Optimal global alignment(upmost):\n";
    for (std::vector<std::string>::size_type i = 0; i < align_1.size(); i++)
        std::cout << align_1[i] << ' ';
    std::cout << "\n";
    for (std::vector<std::string>::size_type i = 0; i < align_2.size(); i++)
        std::cout << align_2[i] << ' ';
    std::cout << "\n";
}

int main() {
    std::ifstream fsim_matrix, fseq_1, fseq_2;
    std::vector<int> seq_1, seq_2;
    int sim;
    std::vector<std::string> align_1, align_2;
    createSimMatrix(fsim_matrix);
    printSimMatrix();
    createSeqArray(fseq_1, seq_1, "sequence_1.txt");
    createSeqArray(fseq_2, seq_2, "sequence_2.txt");
    printSeqs(seq_1, seq_2);
    createDynMatrix(seq_1.size() + 1, seq_2.size() + 1);
    sim = computeSim (seq_1, seq_2);
    printDynMatrix(seq_1.size() + 1, seq_2.size() + 1);
    std::cout << "Similarity: " << sim << "\n";
    align(seq_1.size(), seq_2.size(), 0, align_1, align_2, seq_1, seq_2);
    printAlignment(align_1, align_2);
    deleteSimMatrix();
    deleteDynMatrix(seq_1.size() + 1, seq_2.size() + 1);
}
