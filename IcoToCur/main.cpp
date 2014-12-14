#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


std::string icolocation;
std::string curlocation;
int hsX = 0;//default value 0
int hsY = 0;//default value 0
void printHelp(void);
inline bool exists(const std::string&);
bool ValidateInput(void);
void convert();

int main(int argc, char* argv[]){
		int count;
    // Display each command-line argument.
    for( count = 0; count < argc; count++ ){
		if(count>=argc)
			break;
		 if (std::string(argv[count]) == "-h"){
			 printHelp();
			 break;//help is shown-> no need to take further arguments
		 }
		 if (std::string(argv[count]) == "-ico"){
			 icolocation = std::string(argv[++count]);//increase count as next argument is location of ico
			 std::cout<<count<<" = -ico = "<<icolocation<<"\n";//Debug, to be removed
			 continue;//"ico" was identified->no need for further checks
		 }
		 if (std::string(argv[count]) == "-cur"){
			 curlocation = std::string(argv[++count]);//increase count as next argument is location of cur
		     std::cout<<count<<" = -cur = "<<curlocation<<"\n";
			 continue;//"cur" was identified->no need for further checks
		 }
		 if (std::string(argv[count]) == "-hsX"){
			 std::stringstream(argv[++count])>>hsX;//increase count as next argument is value of hsX
			 std::cout<<count<<" = -hsX = "<<hsX<<"\n";
			 continue;//"hsX" was identified->no need for further checks
		 }
		 if (std::string(argv[count]) == "-hsY"){
			 std::stringstream(argv[++count])>>hsY;//increase count as next argument is value of hsY
			 std::cout<<count<<" = -hsY = "<<hsY<<"\n";
			 continue;//"hsY" was identified->no need for further checks
		 }
	}
	if(ValidateInput()){
		std::cout<<"The provided Input was valid:\n";
		std::cout<<"The icon converted is        :"<<icolocation<<"\n";
		std::cout<<"The cursor will be saved at  :"<<icolocation<<"\n";
		std::cout<<"The Cursor's horizontal HS is: "<<hsX<<"\n";
		std::cout<<"The Cursor's vertical HS is  : "<<hsY<<"\n";
		convert();
	}
	else{
		std::cout<<"You provided invalide Input:\nEither the Input format or the values provided were wrong!\n";
		std::cout<<"You can view the help with parameter -h";
	}
}

void printHelp(){
	std::cout<<"\nValid Parameters\n";
	std::cout<<" -h   | Displays this help\n";
	std::cout<<" -ico | Location of the .ico File (Input)\n";
	std::cout<<" -cur | Location of the .cur File (Output)\n";
	std::cout<<" -hsY | Vertical location of the cursor's hotspot (Default:0)\n";
	std::cout<<" -hsX | Horizontal location of the cursor's hotspot (Default:0)\n";
}

//http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c/12774387#12774387 // version2
inline bool exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else
        return false;
}

bool ValidateInput(){
	bool valid = true;
	if(!exists(icolocation))
		valid=false;
	if(hsX<0||hsX>255)
		valid = false;
	if(hsX<0||hsX>255)
		valid = false;
	return valid;
}

void convert(){
	std::cout<<"Starting conversion with the following parameters:";
	std::cout<<"Icon at      : "<<icolocation<<"\n";
	std::cout<<"Cursor out to: "<<curlocation<<"\n";
	std::cout<<"X-Hotspot at : "<<hsX<<"\n";
	std::cout<<"Y-Hotspot at : "<<hsY<<"\n";
	std::ofstream curfile;//writes the new file
	curfile.open(curlocation, std::ios::binary | std::ios::out | std::ios::app);
	std::ifstream icofile;//gets data from .ico
	icofile.open(icolocation, std::ios::binary | std::ios::in);
	unsigned char* curheader = new unsigned char[6];//contains the bytes that can be written to the .cur file header
	curheader[0]=curheader[1]=curheader[3]=curheader[5]=0;//reserved or part of two byte-value
	curheader[2]=2;//type
	curheader[4]=1;//picturecount, in .cur always 1
	curfile.write((char*)curheader, 6);
	char* icoinfo = new char[];//contains information from the ico file that is needed for the cur file
	icofile.seekg(6);
	icofile.read(icoinfo, 4);//same in ico and cur
	curfile.write(icoinfo, 4);
	curheader = new unsigned char[4];
	curheader[1]=curheader[3]=0;
	curheader[0] = (unsigned char)hsX;
	curheader[2] = (unsigned char)hsY;
	curfile.write((char*)curheader, sizeof(curheader));
	icofile.seekg(0,std::ios::end);
	int bytesLeft = (int)icofile.tellg() - 14;
	icofile.seekg(14);//14 because the index is 0-based
	icoinfo = new char[bytesLeft];
	icofile.read(icoinfo,bytesLeft);
	curfile.write(icoinfo, bytesLeft);
	//close the files*/
	curfile.close();
	icofile.close();
	std::cout<<"\nConversion Successful! Find your cursor at: \n"<<curlocation;
}