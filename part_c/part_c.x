typedef string Path<1000>;


/* Specify the arguments */
struct parameters{
	int number1;
	int number2;
	Path path;
};

/* 
 * 1. Name the program and give it a unique number.
 * 2. Specify the version of the program.
 * 3. Specify the signature of the program.
*/
program PART_C_PROG{
	version PART_C_VERS{
		/* Takes a parameters structure and gives the string result. */
		string part_c(parameters)=1;
	}=1;
}=0x12345678;