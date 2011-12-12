// Only for testing

#include "RotationalOffset.h"

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
  ifstream fin;
  ofstream fon("offsetO.txt");
  string dir, filepath;

  DIR *dp;
  struct dirent *dirp;
  struct stat filestat;
  int count = 0;
  float item, cost;

  vector< vector< pair<unsigned int, unsigned int> > > Contours(cMAX_POINTS);

  cout << "dir to get files of: " << flush;
  getline( cin, dir );  // gets everything the user ENTERs

  dp = opendir( dir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }

  while ((dirp = readdir( dp )))
  {
    filepath = dir + "/" + dirp->d_name;

    // If the file is a directory (or is in some way invalid) we'll skip it
    if (stat( filepath.c_str(), &filestat )) continue;
    if (S_ISDIR( filestat.st_mode ))         continue;


    fin.open( filepath.c_str() );
    while ( !fin.eof() ) {
        fin >> item >>  cost;
        Contours[count].push_back(make_pair((unsigned int)item,(unsigned int)cost));
    }
    count++;
    item=0;
    cost=0;
    fin.close();
   }

	vector< vector<float> > Offset;

	IRO *RO = new cRotationalOffset();

	Offset = RO->GetMinRadius(Contours);

	// Write Offset in OffsetO.text file //
    for(int j=0;j< cMAX_POINTS;j++){

        vector<float>::iterator it2;

        for(it2 = Offset[j].begin();it2 != Offset[j].end();it2++){
            fon << *it2 << "\t";
        }
        fon << endl;
    }



	system("pause");
    closedir( dp );
    fon.close( );
	return(0);
}

