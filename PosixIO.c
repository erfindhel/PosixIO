#include<stdio.h>
#include <fcntl.h>

int DupDemo(const char * first_filename, const char * second_filename)
{
	int fd[4];
	int offset[4];

	printf("\n\n/////////////dup() demonstration://///////////\n");
	//open file twice
	fd[0] = open(first_filename, O_RDONLY);
	fd[1] = open(first_filename, O_RDONLY);
	offset[0] = lseek(fd[0], 10, SEEK_CUR); //shifts from 0 to 10
	offset[1] = lseek(fd[1], 10, SEEK_CUR); //shifts from 0 to 10 Pointer for fd[0] not affected
	printf("\ndescriptor %d pointer position: %d\ndescriptor %d pointer position: %d", fd[0], offset[0], fd[1], offset[1]);

	//dup descriptor of opened file
	fd[2] = open(second_filename, O_RDONLY);
	fd[3] = dup(fd[2]);						//creates alias for fd[2]
	offset[2] = lseek(fd[2], 15, SEEK_CUR); //shifts from 0 to 15
	offset[3] = lseek(fd[3], 15, SEEK_CUR); //shifts from 15 to 30. Pointer for fd[2] shifts too
	printf("\n\ndescriptor %d pointer position: %d\ndescriptor %d pointer position: %d", fd[2], offset[2], fd[3], offset[3]);
	for (int i = 0; i < 4; ++i)
	{
		if (close(fd[i])<0) return 1;
	}
	return 0;
}

int WriteDemo(const char * first_filename, const char * second_filename)
{
	int fd[2];	// array to hold descriptors
	int br;		//number of bytes readed
	char buf[10];

	//process first file
	fd[0] = open(first_filename, O_RDWR);
	char title1[] = "Original text of 1st file:\n";
	write(1, title1, sizeof(title1)-1); // write to standard output
	while ((br = read(fd[0], buf, sizeof(buf))) != 0)
		write(1, buf, br);
	lseek(fd[0], 5, SEEK_SET); //shift pointer to 5th byte
	write(fd[0], "THIS TEXT WAS INSERTED", 22);	// write data starting from 6th byte of file
	char title2[] = "\n\nText of 1st file after lseek(5) writing:\n";
	write(1, title2, sizeof(title2)-1);
	lseek(fd[0], 0, SEEK_SET); //return to the start of file
	while ((br = read(fd[0], buf, 1)) != 0)
		write(1, buf, br);
	close(fd[0]);

	//process second file
	fd[1] = open(second_filename, O_RDWR | O_APPEND);
	char title3[] = "\n\nOriginal text of 2nd file:\n";
	write(1, title3, sizeof(title3)-1);
	while ((br = read(fd[1], buf, sizeof(buf))) != 0)
		write(1, buf, br);
	lseek(fd[1], 5, SEEK_SET);
	write(fd[1], "THIS TEXT WAS INSERTED", 22); //O_APPEND will ignore 5-byte shift
	lseek(fd[1], 0, SEEK_SET);
	char title4[] = "\n\nText of the 2nd file after lseek(5) & O_APPEND writing:\n";
	write(1, title4, sizeof(title4)-1);
	while ((br = read(fd[1], buf, 1)) != 0)
		write(1, buf, br);
	close(fd[1]);
	return 0;
}

int  main(int argc, char *argv[])
{
	char*  first_filename;
	char*  second_filename;
	if (3 == argc)
	{
		first_filename = argv[1];
		second_filename = argv[2];
	}
	else
	{
		first_filename = "file1.txt";
		second_filename = "file2.txt";
	}
	WriteDemo(first_filename, second_filename);
	DupDemo(first_filename, second_filename);
	getchar();
	return 0;
}