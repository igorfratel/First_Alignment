#include <string>
#include <iostream>
#include <fstream>

/*Similarity matrix. Position [5][6] contains the similarity between 5 and 6.*/
int **sim_matrix;
/*Similarity matrix dimension (dim x dim)*/
int dim;

void createMatrix(std::ifstream &fsim_matrix) {
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

void printMatrix() {
    /*Prints the global variale "sim_matrix" to the standard output.*/
    std::cout << "matrix dimensions: "<< dim << "x" << dim;
    for (int i = 0; i < dim; i++) {
        std::cout << "\n";
        for (int j = 0; j < dim; j++)
            std::cout << sim_matrix[i][j] << " ";
    }
    std::cout << "\n";
}

void deleteMatrix() {
    /*Frees space allocated for the global variable "sim_matrix"*/
    for (int i = 0; i < dim ; ++i)
        delete [] sim_matrix[i];
    delete [] sim_matrix;
}

int *createSeqArray(std::ifstream &fseq, int *seq, std::string filename) {
    /*Receives the address to an ifstream (a .txt file), an int pointer and a 
     *string.
     *Reads the first word of the file, which is the sequence length.
     *Allocates the pointer for the sequence array and fills it with data from
     *the file.
     *Returns the filled array.*/
    std::string str_len;
    int len;
    fseq.open(filename);
    fseq >> str_len;
    len = std::stoi(str_len);
    seq = new int[len];
    for (int i = 0; i < len; i++)
        fseq >> seq[i];
    fseq.close();
    return seq;
}

void printSeqs(int *seq_1, int *seq_2) {
    /*Receives two int arrays OF SIZE 5 and prints them to the standard 
     *output.*/
    std::cout << "Sequences:\n";
    for (int i = 0; i < 5; i++)
        std::cout << seq_1[i] << " ";
    std::cout << "\n";
    for (int i = 0; i < 5; i++)
        std::cout << seq_2[i] << " ";
    std::cout << "\n";
}

int main() {
    std::ifstream fsim_matrix;
    std::ifstream fseq_1, fseq_2;
    int *seq_1, *seq_2;
    createMatrix(fsim_matrix);
    printMatrix();
    seq_1 = createSeqArray(fseq_1, seq_1, "sequence_1.txt");
    seq_2 = createSeqArray(fseq_2, seq_2, "sequence_2.txt");
    printSeqs(seq_1, seq_2);
    deleteMatrix();
}
