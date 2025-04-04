# HMMPairwiseAlignment

Project for the "Bioinformatics 2" course at the University of Zagreb, Faculty of Electrical Engineering and Computing.
Course page: [https://www.fer.unizg.hr/en/course/enbio2](https://www.fer.unizg.hr/en/course/enbio2)

**Contributors:** Fran Ostroški & Leon Hegedić

## Usage

1.  **Prepare Input:**
    * Place your desired `.fasta` format file(s) in the `input` folder. (The folder will be created on the first run if it doesn't exist.)

2.  **Extract Sequences:**
    * Run `LoadSequence.py`. Extracted sequences will be saved in a `sequences` folder as `.txt` files.
    * ```bash
        python LoadSequence.py
        ```

3.  **Generate Pairs:**
    * Run `GeneratePairs.py` with the desired arguments.
    * Arguments:
        * `-fn` or `--file_name`: Filename (name of the desired `.txt` file).
        * `-len` or `--desired_length`: Desired length of output parsed pairs, ready to use for the Viterbi and Needleman-Wunsch algorithms.
    * Ensure you are in the `python_files` directory for this to work.
    * Example:
        ```bash
        python GeneratePairs.py -fn sequences/your_sequence.txt -len 100
        ```

4.  **Compile C++ Code:**
    * Compile the C++ code using the following command:
    * ```bash
        g++ -std=c++17 code/main.cpp code/model/decoding.cpp code/model/evaluation.cpp code/model/learning.cpp code/utils/needleman_wunsch.cpp code/utils/utils.cpp -o HMM
        ```

5.  **Run the Program:**
    * The program accepts the following parameters:
        * `-m`: **Required.** Specifies the mode of operation. Available modes:
            * `viterbi`: Runs the Viterbi algorithm to align sequences.
            * `needleman`: Runs the Needleman-Wunsch algorithm to align sequences.
            * `train`: Trains a model using given training data.
        * `-f`: **Required** for `viterbi` and `needleman` modes. Specifies the name of the file containing the sequences to be aligned.
        * `-d`: **Required** for `train` mode. Specifies the name of the folder where the training sequences are stored (either in a single file or multiple files).
        * `-s`: **Optional.** Specifies the name of the file where the results will be saved. In `viterbi` and `needleman` modes, it saves the aligned sequences. In `train` mode, it saves the model parameters.
        * `-b`: **Optional.** Sets the maximum number of iterations for training.
        * `-t`: **Optional.** Specifies the tolerance for the loss during model training.
    * Example usage:
        ```bash
        ./HMM -m viterbi -f sequences/unaligned_seq.txt -s aligned_seq.txt
        ```

## Requirements

* **C++:** C++17 or newer.
* **Python:** Python 3.8 or newer.
* **Python Libraries:**
    * `biopython`: Install using `pip`:
        ```bash
        pip install biopython
        ```
