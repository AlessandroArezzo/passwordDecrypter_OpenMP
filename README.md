# passwordDecrypter_OpenMP
This project has been realized as an mid-term elaborate of the Parallel Computing course at the University of Florence.<br>
The code implements a DES password decrypter in a <b>sequential version in C++ </b> and in a <b>parallel variant</b> created with the <b>Open MP</b> framework.<br>
The sequential implementation applies the brute force algorithm to search for a hash to be decrypted with the encodings of a set of passwords provided in an input .txt file.<br>
The parallel variant similarly applies the same algorithm but by parallelizing the code by means of an approach that divides the data to be processed in chunks based on the number of threads generated.<br>
The two versions are then compared through two differents testing phases: one on words placed in particular positions of the dataset and one on randomly chosen words in the dataset.
The resulting speeedup were then calculated and displayed in output.

<h2>Prerequisites</h2>
To use the code you need to have installed:

<ul>
<li><b>Open SSL library:</b> for using the DES_fcrypt () function in the decryptParallel () method located in the Decrypter.cpp file.</li>
<li><b>Open MP framework:</b> for the parallel implementation</li>
</ul>

<h2>How to use the code</h2>
To execute the code simply supply the following parameters in order:

<ul>
<li>Number of tests to be performed for each word in a characterizing position (first phase of testing). Select a number less than or equal to 0 to ignore this testing phase</li>
<li>Number of tests to be performed for each word randomly chosen in the dataset (second phase of testing). Select a number less than or equal to 0 to ignore this testing phase </li>
<li>Number of words to choose randomly (second phase of testing)</li>
</ul>

<h2>Other information</h2>
The results and other implementation details are available in the report found at the link: <b>(Still to be inserted)</b>
