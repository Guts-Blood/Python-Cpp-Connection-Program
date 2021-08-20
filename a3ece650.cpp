#include <cstring>
#include <vector>
#include<string>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include<sys/wait.h>
using namespace std;
/// Entry point of process A
int procA(char* input) {
  // Process A writing to C

  char *filename = (char*)"./rgen";
  char *arg_list[]={filename,input,NULL};
  execvp("./rgen",arg_list);

  return 0;
}

/// Entry point of process B
int procB(void) {
  // cout<<"in procB"<<endl;
  // do{
  //   string inputfromc;
  //   getline(cin,inputfromc);
  //   cout<<inputfromc<<endl;
  // }while (!cin.eof());

  char *filename = (char*)"./a2ece650";
  char *arg_list[]={filename,NULL};
  execvp("./a2ece650",arg_list);
  
  return 0;
}

/// Entry point of process C
int procC(void) {
  // Process C reading from both A and B
  // cout<<"V 8"<<endl;
  // cout<<"E {<0,2>,<0,3>,<0,4>,<1,3>,<4,7>,<5,2>,<5,6>}"<<endl;
  char *filename = (char*)"python3";
  char *argname = (char*)"a1ece650.py";
  char *arg_list[]={filename,argname,NULL};
  execvp("python3",arg_list);

  return 0;
}

int main(int argc, char * argv[]) {
  
  FILE* stream;
  string input_commands="";
  int ch;
  for(int count=1; count<argc;count++)
  {
      string add = argv[count]; 
      input_commands.append(add);
  }
  char *input = new char[input_commands.length() + 1];
  strcpy(input, input_commands.c_str());

  std::vector<pid_t> kids;
  // create a pipe
  int AtoC[2];
  pipe(AtoC);
  int CtoB[2];
  pipe(CtoB);

  pid_t child_pid;
  child_pid = fork();
  if (child_pid == 0) {
    // Process A writes to Process C
    close(CtoB[0]);
    close(CtoB[0]);
    close(AtoC[0]);
    dup2(AtoC[1], STDOUT_FILENO);
    close(AtoC[1]); // Close this too!
    return procA(input);
  } else if (child_pid < 0) {
    std::cerr << "Error: could not fork\n";
    return 1;
  }
  kids.push_back(child_pid);
  //fork another process
  child_pid = fork();
  if (child_pid == 0) {
    
    // Process C reads from process A
    close(AtoC[1]);
    dup2(AtoC[0], STDIN_FILENO);
    close(AtoC[0]);
    //Process C writes to process B
    close(CtoB[0]);
    dup2(CtoB[1], STDOUT_FILENO);
    close(CtoB[1]);
    return procC();
  }
  else if (child_pid < 0) {
    std::cerr << "Error: could not fork\n";
    return 1;
  }
  kids.push_back(child_pid);

  child_pid = fork();
  if (child_pid == 0) {
    //Process B reads from process C
    close(AtoC[0]);
    close(AtoC[1]);
    close (CtoB[1]);
    dup2(CtoB[0], STDIN_FILENO);
    close(CtoB[0]);
    return procB();
  } else if (child_pid < 0) {
    std::cerr << "Error: could not fork\n";
    return 1;
  }
  kids.push_back(child_pid);
  
  close(AtoC[0]);
  close(AtoC[0]);
  close (CtoB[0]);
  stream = fdopen (CtoB[1], "w");
  getline(cin,input_commands);
  while(!cin.eof()){
  
  char *input2 = new char[input_commands.length() + 1];
  strcpy(input2, input_commands.c_str());
  fprintf (stream, "%s\n",input2);   
  fflush (stream);
  getline(cin,input_commands);
  }
  close (CtoB[1]);
  //cout<<"should end"<<endl;
  // /* Wait for the child process to finish. */
  // waitpid (child_pid, NULL, 0);

  // exit(0);
    
  // kids.push_back(child_pid);
  // child_pid = 0;
  // send kill signal to all children
  // cout<<"sleeping"<<endl;
  // sleep(20);
  int count=0;
  for (pid_t k=kids[count];count<4;count++) {
    int status;
    kill(k, SIGTERM);
    waitpid(k, &status, 0);
   }

  //return res;
}
