//////////////////////////////////////////////////////////////////////
//PURPOSE:
//       Uses a JSON Config to load a given matrix and reduction parameters to reduce a covariance matrix.1
//       Use Config file to determine skipped bins and merge bins then output reduced covariance
//
//CALLING SEQUENCE:
//       ReduceCovar.exe -v 1 -f /path/to/configname
//
//INPUT PARAMETERS:
//      configname: Path to the json configuration file
//
//OPTIONAL INPUT PARAMETERS:
//  Key: flag = [arg type] arg description {default value}
//       (flags without types do not require args: default = false, toggles to true)
//
//  f = [string] config file /path/name
//  v =  comments / verbose {false}
//  h =  print help - print this msg (program will exit if used)
//
//OUTPUTS:
//  Single ROOT File with reduced covariance matrix (TMatrixDSym).
//
//MODIFICATIONS:
//       2023 Written by DMunford + TWood
//////////////////////////////////////////////////////////////////////

//C++
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <json/json.h>

//ROOT
#include "TSystem.h"
#include "TCanvas.h"
#include "TMatrixDSym.h"
#include "TFile.h"
#include "TStyle.h"

using namespace std;

//Function to parse cmd line
void ParseCmdLineOpts(int argc, const char* argv[]);
//Function to parse JSON Config
void LoadConfig(const char* cfg);

//Storing config data
string input_file;
string covar_name;
vector<int> skipped_bins;
vector<vector<int>> combined_bins;

//Cmd line arg global vars
const char* config_filename = "";
bool verbose;

void ParseCmdLineOpts(int argc, const char* argv[]){
    cout << "Input Args: argc = " << argc << endl;
    for(int i = 0; i < argc; i++) cout << "argv["<<i<<"] = " << argv[i] << endl;

    //Input tags
    //f = [string] config file /path/name
    //v = [bool] make comments
    //h = [bool] print help

    verbose = false;
    
    int c;
    while((c = getopt(argc, (char **) argv, ":f:v:h")) != EOF){
        if (c < 0) break;
        switch (c) {
            case 'f':{
                config_filename = optarg;
                break;
            }
            case 'v':{
                verbose = optarg;
                break;
            }
            case 'h':{
                cout<<"How to run ReduceCovar:"<<endl;
                cout<<"  flag = [type:optional?] arg description {default}"<<endl;
                cout<<"  (bool types accept true/false or 1/0 as args, defaults are false, use flag to toggle to true)\n"<<endl;
                cout<<"  f = [string] config file /path/name"<<endl;
                cout<<"  v = [bool:opt]   comments / verbose {false}"<<endl;
                cout<<"  h = [bool:opt]   print help - print this msg (program will exit if used)\n"<<endl;
                exit(1);
            }
        }
    }
    if (((config_filename != NULL) && (config_filename[0] == '\0'))) {
        cout << "Config file was not set...Exiting" << endl;
        exit(1);
    }
}

int main(int argc, const char* argv[]){
    gStyle->SetOptStat(0);
    cout << "RC - hello world" << endl;
    ParseCmdLineOpts(argc, argv);
    LoadConfig(config_filename);

    // Step 1: Load File and Covariance Matrix
    TFile* covar_file = new TFile(input_file.c_str());

    TMatrixDSym* covariance_in = (TMatrixDSym*)covar_file->Get(covar_name.c_str());
    TMatrixDSym covariance = *covariance_in;
    if(verbose) covariance.Print();

    int cov_size = covariance.GetNcols();

    vector<int> bin_changes(cov_size, 0);

    for(int i = 0; i < skipped_bins.size(); i++) bin_changes[skipped_bins[i]] = -1;
    for(int i = 0; i < combined_bins.size(); i++){
        for(int j = 1; j < combined_bins[i].size(); j++){
            bin_changes[combined_bins[i][j]] = -1;
        }
    }
    int index = 0;
    for(int i = 0; i < bin_changes.size(); i++){
        if(bin_changes[i] == -1) continue;
        bin_changes[i] = index;
        index++;
    }
    for(int i = 0; i < combined_bins.size(); i++){
        for(int j = 1; j < combined_bins[i].size(); j++){
            bin_changes[combined_bins[i][j]] = bin_changes[combined_bins[i][0]];
        }
    }

    int new_cov_size;
    new_cov_size = cov_size - skipped_bins.size();
    for(int i = 0; i < combined_bins.size(); i++){
        new_cov_size -= (combined_bins[i].size() - 1);
    }

    TMatrixDSym new_covar(new_cov_size);
    for(int x = 0; x < cov_size; x++){
        if(bin_changes[x] == -1) continue;
        int new_x = bin_changes[x];
        for(int y = 0; y < cov_size; y++){
            if(bin_changes[y] == -1) continue;
            int new_y = bin_changes[y];
            new_covar[new_x][new_y] += covariance[x][y];
        }
    }
    if(verbose) new_covar.Print();

    string output_filename;
    size_t lastdot = input_file.find_last_of(".");
    if (lastdot == string::npos) output_filename = input_file;
    else output_filename = input_file.substr(0, lastdot);
    output_filename = output_filename+"_reduced.root";

    TFile* output_file = new TFile(output_filename.c_str(), "RECREATE");
    string red_covname = "reduced_"+covar_name;
    new_covar.Write(red_covname.c_str());
    output_file->Close();
    return 0;
}

void LoadConfig(const char* cfg){
    ifstream json_file(cfg, ifstream::binary);
    Json::Value conf;
    json_file >> conf;

    input_file = conf["inputfile"].asString();
    covar_name = conf["covarname"].asString();

    const Json::Value& ignore_tmp = conf["ignorebins"];
    for(int i = 0; i < ignore_tmp.size(); i++){
        skipped_bins.push_back(ignore_tmp[i].asInt());
    }
    
    const Json::Value& combine_tmp1 = conf["combinebins"];
    for(int i = 0; i < combine_tmp1.size(); i++){
        const Json::Value& combine_tmp2 = combine_tmp1[i];
        vector<int> tmp_combine;
        for(int j = 0; j < combine_tmp2.size(); j++){
            tmp_combine.push_back(combine_tmp2[j].asInt());
        }
        combined_bins.push_back(tmp_combine);
    }
}