#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#define GAP -2

/*Similarity matrix. eg: Position [5][6] contains the similarity between 5 and 6.*/
int **sim_matrix;
/*Similarity matrix dimension (dim x dim)*/
int dim;
/*Dynamic programming matrix*/
int **dyn_matrix;

struct sequence {
    int *symbols;
    int len;
};

void createSimMatrix(std::ifstream &fsim_matrix) {
    /*Receives the address to an ifstream (a .txt file).
     *Reads the first word of the file, which is the matrix dimension.
     *Fills the global variable matrix "sim_matrix" based on the numbers 
     *contained in the file.*/
    std::string str_dim;
    fsim_matrix.open("similarity_matrix.txt");
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

void createSeqArray(std::ifstream &fseq, sequence *seq,
                    std::string filename) {
    /*Receives the address to an ifstream (a .txt file), a pointer to a sequence
     *struct and a string.
     *Reads the first word of the file, which is the sequence length.
     *Allocates the pointer for the sequence array and fills it with data from
     *the file.
     *Returns the filled array.*/
    std::string str_len;
    fseq.open(filename);
    fseq >> str_len;
    seq->len = std::stoi(str_len);
    seq->symbols = new int[seq->len];
    for (int i = 0; i < seq->len; i++)
        fseq >> seq->symbols[i];
    fseq.close();
}

void printSeqs(sequence seq_1, sequence seq_2) {
    /*Receives two sequence structs and prints their symbols attribute to the 
     *standard output.*/
    std::cout << "Sequences:\n";
    for (int i = 0; i < seq_1.len; i++)
        std::cout << seq_1.symbols[i] << " ";
    std::cout << "\n";
    for (int i = 0; i < seq_2.len; i++)
        std::cout << seq_2.symbols[i] << " ";
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

int computeSim (sequence seq_1, sequence seq_2, int gap) {
    /*Receives two sequences and a gap penalty.
     *Computes and returns the global similarity between the sequences using the
     *gap penalty and the sim_matrix.*/
    int match;
    int options[3];
    for (int i = 0; i <= seq_1.len; i++)
        dyn_matrix[i][0] = i * gap;
    for (int i = 0; i <= seq_2.len; i++)
        dyn_matrix[0][i] = i * gap;
    for (int i = 1; i <= seq_1.len; i++)
        for (int j = 1; j <= seq_2.len; j++) {
            match = sim_matrix[seq_1.symbols[i-1]-1][seq_2.symbols[j-1]-1];
            options[0] = dyn_matrix[i-1][j] + gap;
            options[1] = dyn_matrix[i-1][j-1] + match;
            options[2] = dyn_matrix[i][j-1] + gap;
            dyn_matrix[i][j] = *std::max_element(options, options + 3);
        }
    return dyn_matrix[seq_1.len][seq_2.len];
}

int main() {
    std::ifstream fsim_matrix;
    std::ifstream fseq_1, fseq_2;
    sequence seq_1, seq_2;
    int sim;
    createSimMatrix(fsim_matrix);
    printSimMatrix();
    createSeqArray(fseq_1, &seq_1, "sequence_1.txt");
    createSeqArray(fseq_2, &seq_2, "sequence_2.txt");
    printSeqs(seq_1, seq_2);
    createDynMatrix(seq_1.len + 1, seq_2.len + 1);
    sim = computeSim (seq_1, seq_2, GAP);
    printDynMatrix(seq_1.len + 1, seq_2.len + 1);
    std::cout << "Similarity: " << sim << "\n";
    deleteSimMatrix();
    deleteDynMatrix(seq_1.len, seq_2.len);
}
