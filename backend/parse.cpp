#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>

#define BUFFER_SIZE 1024

using namespace std;

string formatDate(string date);
void beginJSON(ofstream & file);
void endJSON(ofstream & file);
void beginPrinter(ofstream & file, string name, string status, int first);
void endPrinter(ofstream & file);
void noQueue(ofstream & file);
void beginQueue(ofstream & file);
void addToQueue(ofstream & file, string filename, string rank, string owner, string size, int first);
void endQueue(ofstream & file);
string parsedStatus(string status);
string removeBadChars(string name);
void printTime();


int main(int argc, char * argv[])
{

  if(argc < 3) // Correct usage
  {
    cerr << "Usage: " << argv[0] << " output_file printer1 printer2 ... printer n" << endl;
    return -1;
  }

  // Let's open the output file we'll be working on!
  char output_file[50];
  sprintf(output_file,"%s.json",argv[1]);
  ofstream jsonfile(output_file);

  // Let's generate the beginning of the json file here
  beginJSON(jsonfile);
  
  FILE * fp;
  string in;
  char buffer[BUFFER_SIZE], file_name[255], *ptr;
  int i;
  
  for(i=2;i<argc;i++) // we need to run lpq on each printer
  {
    string rank, owner, filename, size, jobid, status="";
    char printerName[20], command[100], *token, *ptr;
    int onJobs = 0, firstJob = 1, end_len;
    sprintf(command,"lpq -P%s",argv[i]);

    /* run the lpq command */
    if ( (fp = popen(command,"r") ) == NULL)
    {
      printf("Failed to run command: %s\n",command);
      sprintf(command,"rm %s.json",argv[1]);
      system(command);
      return 1;
    }

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
    {
      if(!onJobs && !strcmp(buffer,"no entries\n") )
      {
	// no entries
	/* JSON file stuff */
	if(i == 2)
	  beginPrinter(jsonfile,argv[i],status,1);
	else
	  beginPrinter(jsonfile,argv[i],status,0);
      }
      else if(!onJobs && !strcmp(buffer,"Rank   Owner      Job  Files                                 Total Size\n"))
      {
	// header
	onJobs = 1;
	/* JSON file stuff */
	if(i == 2)
	  beginPrinter(jsonfile,argv[i],status,1);
	else
	  beginPrinter(jsonfile,argv[i],status,0);
	beginQueue(jsonfile);
      }
      else if(!onJobs)
      {
	// Error, warning, info messages
	// Need to handle LPQ failures here.
	if(strstr(buffer,"pserv-") != NULL) //LPQ Failed. 
	{
	  sprintf(command,"rm %s.json",argv[1]);
	  system(command);
	  return 1;
        }
	else 
	{
	  buffer[strlen(buffer)-1] = '0';
	  status.assign(buffer);
	}
      }
      else
      {
	// On a job.

	/* Rank */
	token = strtok_r(buffer," ",&ptr);
	rank.assign(token);

	/* owner */
	token = strtok_r(ptr," ",&ptr);
	owner.assign(token);

	/* job id */
	token = strtok_r(ptr," ",&ptr);
	jobid.assign(token);

	/* trim white space before file name */
	while(*ptr == ' ')
	  ptr++;

	/* file name catcher */
	end_len = strlen(ptr)-39;
	token = strtok_r(ptr," ",&ptr);
	strncpy(file_name,"\0",2);

	while(end_len < strlen(ptr))
	{
	  if(strlen(file_name))
	    sprintf(file_name,"%s %s",file_name,token);
	  else
	    sprintf(file_name,"%s",token);
	  token = strtok_r(ptr," ",&ptr);
	}
	filename.assign(file_name);

	/* size of file in bytes */
	size.assign(token);

	if(firstJob)
	{
	  addToQueue(jsonfile,filename,rank,owner,size,1);
	  firstJob = 0;
	}
        else
          addToQueue(jsonfile,filename,rank,owner,size,0);

      }
    }

    /* Either close the queue or put in an empty message */
    if(onJobs)
      endQueue(jsonfile);
    else
      noQueue(jsonfile);

    /* close the printer */
    endPrinter(jsonfile);
  }

  // Let's generate the end of the JSON file here
  endJSON(jsonfile);

  return 0;
}


string formatDate(string date)
{
  int start = 0;
  int current = 0;
  int pos = 0;
  string d[5];
  string output;
  for(current = 0; current < date.length(); current++)
  {
    if(date[current] == ' ' && current - start != 0)
    {
      d[pos++] = date.substr(start,current-start);
      start = current+1;
    }
    else if(date[current] == ' ' && current-start == 0)
      start++;
  }
  d[pos] = date.substr(start,current-start);

  if(d[0] == "Sun")
    output += "Sunday ";
  else if(d[0] == "Mon")
    output += "Monday ";
  else if(d[0] == "Tue")
    output += "Tuesday ";
  else if(d[0] == "Wed")
    output += "Wednesday ";
  else if(d[0] == "Thu")
    output += "Thursday ";
  else if(d[0] == "Fri")
    output += "Friday ";
  else if(d[0] == "Sat")
    output += "Saturday ";
  else
    output = d[0]+" ";

  if(d[1] == "Jan")
    output += "January ";
  else if(d[1] == "Feb")
    output += "February ";
  else if(d[1] == "Mar")
    output == "March ";
  else if(d[1] == "Apr")
    output += "April ";
  else if(d[1] == "May")
    output += "May ";
  else if(d[1] == "Jun")
    output += "June ";
  else if(d[1] == "Jul")
    output += "July ";
  else if(d[1] == "Aug")
    output += "August ";
  else if(d[1] == "Sep")
    output += "September ";
  else if (d[1] == "Oct")
    output += "October ";
  else if(d[1] == "Nov")
    output += "November ";
  else if(d[1] == "Dec")
    output += "December ";
  else
    output += d[1] + " ";

  output += d[2]+" ";

  if(d[3][2] == ':')
  {
    int hour = atoi(d[3].substr(0,2).c_str());
    if(hour > 12)
    {
      stringstream out;
      out << (hour-12);
      output += out.str();
      output += d[3].substr(2);
    }
    else if(hour == 0)
    {
      output += "12";
      output += d[3].substr(2);
    }
    else
      output += d[3];
    if(hour < 12)
      output += " AM ";
    else
      output += " PM ";
  }
  else
  {
    output += d[3] + " AM ";
  }

  output += d[4];

  return output;
}
void beginJSON(ofstream & file)
{
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  string time_stamp = asctime (timeinfo);
  time_stamp = time_stamp.substr(0,time_stamp.length()-1);
  file << "{\"printers\":{\"timestamp\":\""<<formatDate(time_stamp)<<"\",\"printer\":[";
}
void printTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  string time_stamp = asctime (timeinfo);
  time_stamp = time_stamp.substr(0,time_stamp.length()-1);
  cerr << time_stamp << endl;
}
void endJSON(ofstream & file)
{
  file << "]}}";
}
void beginPrinter(ofstream & file, string name, string status, int first)
{
  if(!first)
    file << ",";
  file << "{\"name\": \""<<name<<"\",\"status\":\""<<removeBadChars(status)<<"\",\"queue\": ";
}
void endPrinter(ofstream & file)
{
  file << "}";
}
void noQueue(ofstream & file)
{
  file << "\"empty\"";
}
void beginQueue(ofstream & file)
{
  file << "{\"job\": [";
}
void addToQueue(ofstream & file, string filename, string rank, string owner, string size, int first)
{
  if(!first)
    file << ",";
  file << "{\"file\": \"" << removeBadChars(filename) << "\",\"rank\": \"" << rank << "\",\"owner\": \"" << owner << "\",\"size\": \"" << size << "\"}";
}
void endQueue(ofstream & file)
{
  file << "]}";
}

string removeBadChars(string name)
{
  int i;
  string temp;
  for(i = 0; i < name.length(); i++)
    {
      temp += name[i];
      if(name[i] == '\\')
	temp += "\\";
    }
  return temp;
}
