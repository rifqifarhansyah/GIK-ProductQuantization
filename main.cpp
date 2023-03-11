#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

// generate random vector with k total_vectors(number of database vectors) and n vector_dim (dimension of each vector) with int type
void generate_random_vector(vector<vector<int>>& v, int n, int k) {
    // resize the vector to k vectors of n dimensions
    v.resize(k, vector<int>(n));

    // create a random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 10);

    // fill the vector with random int values
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            v[i][j] = dis(gen);
        }
    }
}

vector<vector<vector<int>>> PQ_train(vector<vector<int>> vectors, int M, int k) {
    int s = vectors[0].size() / M; // segment length
    vector<vector<vector<int>>> codebooks(M, vector<vector<int>>(k, vector<int>(s)));
    
    for (int m = 0; m < M; m++) {
        vector<vector<int>> subvectors(vectors.size(), vector<int>(s));
        for (int i = 0; i < vectors.size(); i++) {
            for (int j = 0; j < s; j++) {
                subvectors[i][j] = vectors[i][m*s + j];
            }
        }
        
        //kmeans2(subvectors, k, codebooks[m], label); // kmeans2 is a function that returns the codebook and label
        //implement kmeans2 here
        vector<int> label(subvectors.size(), -1);
        
    }    
    return codebooks;
}

// vector<vector<int>> PQ_encode(vector<vector<int>> vectors, vector<vector<vector<int>>> codebooks) {
//     int M = codebooks.size();
//     int k = codebooks[0].size();
//     int s = codebooks[0][0].size();
//     vector<vector<int>> PQ_code(vectors.size(), vector<int>(M));
    
//     for(int m=0; m<M; m++){
//         vector<vector<int>> subvectors(vectors.size(), vector<int>(s));
//         for (int i = 0; i < vectors.size(); i++) {
//             for (int j = 0; j < s; j++) {
//                 subvectors[i][j] = vectors[i][m*s + j];
//             }
//         }
//     }
// }

// vector<vector<int>> PQ_decode(vector<vector<int>> codes, vector<vector<vector<int>>> codebooks) {
//     int M = codebooks.size();
//     int k = codebooks[0].size();
//     int s = codebooks[0][0].size();
//     vector<vector<int>> vectors(codes[0].size(), vector<int>(M));
    
//     for (int i = 0; i < codes.size(); i++) {
//         for (int m = 0; m < M; m++) {
//             int c = codes[i][m];
//             for (int j = 0; j < s; j++) {
//                 vectors[i][m*s + j] = codebooks[m][c][j];
//             }
//         }
//     }
//     return vectors;
// }

// vector<float> PQ_search(vector<int> query_vector, vector<vector<vector<int>>> codebook, vector<vector<int>> PQ_code) {
//     int M = codebook.size();
//     int k = codebook[0].size();
//     int s = codebook[0][0].size();
//     vector<vector<float>> distances_table(M, vector<float>(k));
//     for(int m = 0; m < M; m++) {
//         for(int j = 0; j < k; j++) {
//             vector<int> subvector(s);
//             for(int i = 0; i < s; i++) {
//                 subvector[i] = query_vector[m*s + i];
//             }
//             distances_table[m][j] = euclidean_distance(subvector, codebook[m][j]);
//         }
//     }

//     int n = PQ_code.size();
//     int d = PQ_code[0].size();
//     //transpose the distance_tables
//     vector<vector<float>> distances_table_transpose(d, vector<float>(n));
//     for(int i = 0; i < n; i++) {
//         for(int j = 0; j < d; j++) {
//             distances_table_transpose[j][i] = distances_table[j][PQ_code[i][j]];
//         }
//     }

//     vector<float> distances(n);
//     for(int i = 0; i < n; i++) {
//         float sum = 0;
//         for(int j = 0; j < d; j++) {
//             sum += distances_table_transpose[j][i];
//         }
//         distances[i] = sum;
//     }
//     return distances;
// }

// main function for generating random vector and print it
int main() {
    vector<vector<int>> v;
    int dimension = 10;
    int number_of_database = 3;
    int number_of_segments = 8;
    int number_os_centroids = 16;
    
    generate_random_vector(v, dimension, number_of_database);
    
    // print the resulting vector
    cout << "The resulting vector is:" << endl;
    for (const auto& row : v) {
        for (const auto& val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    vector<int> query_vector = v[0];

    // vector<vector<vector<int>>> codebooks = PQ_train(v, number_of_segments, number_os_centroids);
    // cout << "codebooks successfull" << endl;
    // vector<vector<int>> codes = PQ_encode(v, codebooks);
    // cout << "codes successfull" << endl;
    // vector<vector<int>> decoded_vectors = PQ_decode(codes, codebooks);
    // cout << "decoded_vectors successfull" << endl;
    // vector<float> distances = PQ_search(v[0], codebooks, codes);
    // cout << "distances successfull" << endl;
    
    // cout << "The resulting codebooks is:" << endl;
    // for (const auto& row : codebooks) {
    //     for (const auto& val : row) {
    //         for (const auto& val2 : val) {
    //             cout << val2 << " ";
    //         }
    //         cout << endl;
    //     }
    //     cout << endl;
    // }

    // cout << "The resulting codes is:" << endl;
    // for (const auto& row : codes) {
    //     for (const auto& val : row) {
    //         cout << val << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "The resulting decoded_vectors is:" << endl;
    // for (const auto& row : decoded_vectors) {
    //     for (const auto& val : row) {
    //         cout << val << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "The resulting distances is:" << endl;
    // for (const auto& val : distances) {
    //     cout << val << " ";
    // }
    // cout << endl;

    return 0;
}