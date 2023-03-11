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

void kmeans2(vector<vector<int>>& vectors, int k, vector<vector<int>>& codebook, vector<int>& labels) {
    int N = vectors.size(); // number of vectors
    int D = vectors[0].size(); // dimensionality of vectors

    // Initialize centroids randomly
    codebook.resize(k, vector<int>(D));
    for (int i = 0; i < k; i++) {
        int r = rand() % N; // choose random vector index
        for (int j = 0; j < D; j++) {
            codebook[i][j] = vectors[r][j]; // copy vector to centroid
        }
    }

    // Iterate until convergence or max_iterations
    int max_iterations = 100;
    for (int iter = 0; iter < max_iterations; iter++) {
        // Assign each vector to the nearest centroid
        labels.resize(N);
        for (int i = 0; i < N; i++) {
            int best_index = 0; // index of closest centroid
            int best_dist = INT_MAX; // distance to closest centroid
            for (int j = 0; j < k; j++) {
                int dist = 0;
                for (int d = 0; d < D; d++) {
                    int diff = vectors[i][d] - codebook[j][d]; // calculate distance between vector i and centroid j 
                    dist += diff * diff; // accumulate distance squared over all dimensions
                }
                if (dist < best_dist) { // if distance is better than best so far, update index and distance
                    best_index = j;
                    best_dist = dist;
                }
            }
            labels[i] = best_index; // assign vector i to the best centroid
        }

        // Update centroids to the mean of assigned vectors
        vector<vector<int>> new_codebook(k, vector<int>(D)); // initialize new centroids to all zeros
        vector<int> counts(k, 0); // initialize counts of vectors assigned to each centroid to zero
        for (int i = 0; i < N; i++) {  // iterate over all vectors
            int j = labels[i]; // get index of centroid that vector i was assigned to
            for (int d = 0; d < D; d++) {
                new_codebook[j][d] += vectors[i][d];  // add vector i to centroid j
            }
            counts[j]++; // increment count of vectors assigned to centroid j
        }
        for (int j = 0; j < k; j++) { // iterate over all centroids
            if (counts[j] > 0) { // if centroid j has at least one assigned vector
                for (int d = 0; d < D; d++) {
                    codebook[j][d] = new_codebook[j][d] / counts[j];
                }
            }
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
        
        // Call kmeans2 function to get codebook and labels
        vector<int> labels;
        kmeans2(subvectors, k, codebooks[m], labels);
    }    
    return codebooks;
}


vector<vector<int>> PQ_encode(const vector<vector<int>>& vectors, const vector<vector<vector<int>>>& codebook) {
    const int M = codebook.size();
    const int k = codebook[0].size();
    const int s = codebook[0][0].size();
    
    vector<vector<int>> PQ_code(vectors.size(), vector<int>(M));
    
    for (int m = 0; m < M; m++) {
        vector<vector<int>> sub_vectors(vectors.size(), vector<int>(s));
        for (int i = 0; i < vectors.size(); i++) {
            for (int j = 0; j < s; j++) {
                sub_vectors[i][j] = vectors[i][m*s + j];
            }
        }
        
        vector<int> centroid_ids(sub_vectors.size());
        for (int i = 0; i < sub_vectors.size(); i++) {
            int min_dist = INT_MAX;
            int min_idx = 0;
            for (int j = 0; j < k; j++) {
                int dist = 0;
                for (int l = 0; l < s; l++) {
                    dist += pow(sub_vectors[i][l] - codebook[m][j][l], 2);
                }
                if (dist < min_dist) {
                    min_dist = dist;
                    min_idx = j;
                }
            }
            centroid_ids[i] = min_idx;
        }
        
        for (int i = 0; i < centroid_ids.size(); i++) {
            PQ_code[i][m] = static_cast<int>(centroid_ids[i]);
        }
    }
    
    return PQ_code;
}

vector<vector<int>> PQ_decode(vector<vector<int>> codes, vector<vector<vector<int>>> codebooks) {
    int M = codebooks.size();
    int k = codebooks[0].size();
    int s = codebooks[0][0].size();
    vector<vector<int>> vectors(codes[0].size(), vector<int>(M));
    
    for (int i = 0; i < codes.size(); i++) {
        for (int m = 0; m < M; m++) {
            int c = codes[i][m];
            for (int j = 0; j < s; j++) {
                vectors[i][m*s + j] = codebooks[m][c][j];
            }
        }
        
    }
    return vectors;
}

float euclidean_distance(vector<int> v1, vector<int> v2) {
    float sum = 0;
    for (int i = 0; i < v1.size(); i++) {
        sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(sum);
}

vector<float> PQ_search(vector<int> query_vector, vector<vector<vector<int>>> codebook, vector<vector<int>> PQ_code) {
    int M = codebook.size();
    int k = codebook[0].size();
    int s = codebook[0][0].size();
    vector<vector<float>> distances_table(M, vector<float>(k));
    for(int m = 0; m < M; m++) {
        for(int j = 0; j < k; j++) {
            vector<int> subvector(s);
            for(int i = 0; i < s; i++) {
                subvector[i] = query_vector[m*s + i];
            }
            distances_table[m][j] = euclidean_distance(subvector, codebook[m][j]);
        }
    }

    int n = PQ_code.size();
    int d = PQ_code[0].size();
    //transpose the distance_tables
    vector<vector<float>> distances_table_transpose(d, vector<float>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < d; j++) {
            distances_table_transpose[j][i] = distances_table[j][PQ_code[i][j]];
        }
    }

    vector<float> distances(n);
    for(int i = 0; i < n; i++) {
        float sum = 0;
        for(int j = 0; j < d; j++) {
            sum += distances_table_transpose[j][i];
        }
        distances[i] = sum;
    }
    return distances;
}

// main function for generating random vector and print it
int main() {
    vector<vector<int>> v;
    int dimension = 8;
    int number_of_database = 5;
    int number_of_segments = 8;
    int number_os_centroids = 16;
    generate_random_vector(v, dimension, number_of_database);
    
    // print the database vector
    cout << "The database vector is:" << endl;
    for (const auto& row : v) {
        for (const auto& val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }

    vector<int> query_vector = v[1];

    cout << "================================================================================================================================================================" << endl;
    cout << "TRAINING PHASE" << endl;
    vector<vector<vector<int>>> codebooks = PQ_train(v, number_of_segments, number_os_centroids);
    cout << ">> codebooks was successfully created" << endl;
    cout << "ENCODE PHASE" << endl;
    vector<vector<int>> encoded_vectors = PQ_encode(v, codebooks);
    cout << ">> encoded_vectors was successfully created" << endl;
    cout << "DECODE PHASE" << endl;
    vector<vector<int>> decoded_vectors = PQ_decode(encoded_vectors, codebooks);
    cout << ">> decoded_vectors was successfully created" << endl;
    cout << "DISTANCE CALCULATION PHASE" << endl;
    vector<float> distances = PQ_search(query_vector, codebooks, encoded_vectors);
    cout << ">> distances was successfully calculated" << endl;
    cout << "================================================================================================================================================================" << endl;
    cout << "The resulting codebooks is:" << endl;
    for (const auto& row : codebooks) {
        for (const auto& val : row) {
            for (const auto& val2 : val) {
                cout << val2 << "\t";
            }
        }
        cout << endl;
    }

    cout << "The resulting encoded_vectors is:" << endl;
    for (const auto& row : encoded_vectors) {
        for (const auto& val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }

    cout << "The resulting decoded_vectors is:" << endl;
    for (const auto& row : decoded_vectors) {
        for (const auto& val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }

    cout << "The resulting distances is:" << endl;
    int min=100000;
    int index=1;
    int indexResult=1;
    for (const auto& val : distances) {
        cout << val << "\t";
        if(val<min){
            min=val;
            indexResult=index;
        }
        index++;
    }
    cout << endl;

    cout << "The query vector is similar to the vector number " << indexResult << " with distance " << min << endl;
    return 0;
}