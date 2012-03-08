#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /* use -lpthread on the gcc line */
#include <iostream>

using namespace std;

#define LOCATIONS 14 // total number of locations in array.

void * runThread(void * arg);

int main()
{
  int i;

  string args[LOCATIONS] = {
    "vcc vcnrthlw vcsoutp3 vcsoutlw vcsoutb vcpap3 vcpalw vcplt vcpltg vcpltcf vc750let vc750ovr vclw vcblw vclbl30 vcph3let vc7750b",
    "union ru3120lw ru3120cl ru3120cb ru2fwdlw",
    "dorms bh1a09lw balw bllw brlw crlw cylw halw hu2lw nalw ptlw walw",
    "library li1f1lw li2f1lw li2f2p3 li3f1lw li4f1lw",
    "sage sa2704lw sa2715lw sa3101lw sa3101p3 sa4101lw sa4510lw sa5101lw",
    "cii ci3112lw ci3116lw ci3130lw ci3130p3 ci4034lw",
    "jec ec3207lw ec3232lw ec4201l1 ec4201l2",
    "greene gr306lw gr306cb gr306cl gr306cbg",
    "lally la02lw la102lw la104lw",
    "pittsburgh pi1002lw pi1002p3 pi4114lw pi4206lw",
    "troy tr2012lw tr2012p3 tr2015lw tr2018lw",
    "ae ae215lw ae216lw",
    "mrc mrc136lw",
    "walker wl5113lw wl6113lw"
  };

  string loc[LOCATIONS] = {
    "vcc","union","dorms","library","sage","cii","jec","greene","lally","pittsburgh","troy","ae","mrc","walker"
  };

  pthread_t tid[LOCATIONS];
  
  cerr << "Creating threads" << endl;

  for( i = 0; i < LOCATIONS; i++) // run an instance of parse on each set of args
  {
    char * t = (char*) malloc(sizeof(char)*args[i].length()+1);
    strcpy(t,args[i].c_str());
    int rc = pthread_create(&tid[i], NULL, runThread, t);
    if(rc != 0)
      cerr << "Error creating thread" << endl;
  }

  cerr << "Waiting for threads to complete" << endl;

  for(i = 0; i < LOCATIONS; i++) // Wait for all threads to complete
    pthread_join(tid[i], NULL);

  cerr << "All threads completed" << endl;
  
  // Delete data.tar.gz file
  system("rm data.tar.gz");

  // Create data.tar.gz containing all json files
  string tarCommand = "tar cvf data.tar";
  for(i = 0; i < LOCATIONS; i++)
  {
    tarCommand += " "+loc[i] + ".json";
  }
  tarCommand += " >> /dev/null";
  system(tarCommand.c_str());
  system("gzip -9 data.tar");
  
  // Delete all json files
  for(i = 0; i < LOCATIONS; i++)
  {
    string rmCommand = "rm "+loc[i]+".json";
    system(rmCommand.c_str());
  }
  
  return 0;
}
void * runThread(void * arg)
{
  char * a = (char*)arg;
  char command[200];
  strcpy(command, "./parse.out ");
  strcat(command,a);
  system(command);
  return NULL;
}
