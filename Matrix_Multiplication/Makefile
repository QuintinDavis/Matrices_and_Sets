all :
	c++ -O3 -march=native MatrixMultTime.cpp MMStandard.cpp MatrixRowMajor.cpp -o mm_row_standard
	c++ -O3 -march=native MatrixMultTime.cpp MMTranspose.cpp MatrixRowMajor.cpp -o mm_row_transpose
	c++ -O3 -march=native MatrixMultTime.cpp MMRecursive.cpp MatrixRowMajor.cpp -o mm_row_recursive
	c++ -O3 -march=native MatrixMultTime.cpp MMStrassen.cpp MatrixRowMajor.cpp -o mm_row_strassen

	c++ -O3 -march=native MatrixMultTime.cpp MMStandard.cpp MatrixColMajor.cpp -o mm_col_standard
	c++ -O3 -march=native MatrixMultTime.cpp MMTranspose.cpp MatrixColMajor.cpp -o mm_col_transpose
	c++ -O3 -march=native MatrixMultTime.cpp MMRecursive.cpp MatrixColMajor.cpp -o mm_col_recursive
	c++ -O3 -march=native MatrixMultTime.cpp MMStrassen.cpp MatrixColMajor.cpp -o mm_col_strassen

	c++ -O3 -march=native MatrixMultTime.cpp MMStandard.cpp MatrixMorton.cpp -o mm_morton_standard
	c++ -O3 -march=native MatrixMultTime.cpp MMTranspose.cpp MatrixMorton.cpp -o mm_morton_transpose
	c++ -O3 -march=native MatrixMultTime.cpp MMRecursive.cpp MatrixMorton.cpp -o mm_morton_recursive
	c++ -O3 -march=native MatrixMultTime.cpp MMStrassen.cpp MatrixMorton.cpp -o mm_morton_strassen
