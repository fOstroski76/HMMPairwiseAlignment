# HMMPairwiseAlignment


Project regarding the "Bioinformatics 2" course at the University of Zagreb, Faculty of Electrical Engineering and Computing.
Course page: https://www.fer.unizg.hr/en/course/enbio2

Project contributors: Fran Ostroški & Leon Hegedić


## Usage:

1. Place your desired .fasta format file(s) in the input folder (will be created on first run if it doesnt exist)
  
2. Run LoadSequence.py , extracted sequences will be saved in a "sequences" folder as .txt files. 
   
3. Run GeneratePairs.py with desired arguments:
   -fn --file_name: filename, name of desired .txt file
   -len --desired_length: desired length of output parsed pairs, ready to use for Viterbi and Needleman-Wunsch algorithm

   You must be in python_files directory for it to work.

5. Compile the c++ code with the following isntructions: 
   g++ -std=c++17 code/main.cpp code/model/decoding.cpp code/model/evaluation.cpp code/model/learning.cpp code/utils/needleman_wunsch.cpp code/utils/utils.cpp -o HMM 

6. The program parameters:
   -m: Required. Specifies the mode in which the program operates. There are three available modes:
      viterbi: Runs the Viterbi algorithm to align sequences.
      needleman: Runs the Needleman-Wunsch algorithm to align sequences.
      train: Trains a model using given training data.
   -f: Required for viterbi and needleman modes. Specifies the name of the file containing the sequences to be aligned.
   -d: Required for train mode. Specifies the name of the folder where the training sequences are stored, either in a single file or multiple files.
   -s: Optional. Specifies the name of the file where the results will be saved. In viterbi and needleman modes, it saves the aligned sequences to the file. In train mode, it saves the model parameters to the file.
   -b: Optional. Sets the maximum number of iterations for training.
   -t: Optional. Specifies the tolerance for the loss during model training.

   Example: ./HMM -m viterbi -f unaligned_seq.txt -s aligned_seq.txt



##Requirements: 

C++ 17 or newer

Python 3.8 or newer

Python biopython library (pip install biopython)
