#include "Permutation_TMVA.h"

ClassImp(Permutation_TMVA);

//////////

Permutation_TMVA::Permutation_TMVA(const TString &a_era, const TString &a_channel, const int &a_n_jet, const bool &a_chk_pre_cut, const bool &a_chk_final_kin)
{
  cout << "[Permutation_TMVA::Permutation_TMVA]: Init analysis" << endl;

  reduction = 1;

  era = a_era;
  channel = a_channel;
  n_jet = a_n_jet;
  chk_pre_cut = a_chk_pre_cut;
  chk_final_kin = a_chk_final_kin;

  cout << "Era = " << era << ", Channel = " << channel << ", N_Jets = " << n_jet << ", Chk_Pre_Cut = " << chk_pre_cut << ", Chk_Final_Kin = " << chk_final_kin << endl;

  TMVA::gConfig().GetVariablePlotting().fNbins1D = 500;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunPermutationTree/";

  fin = new TFile(path_base + "Vcb_TTLJ_WtoCB_powheg.root");
  tree_correct = (TTree *)fin->Get("Permutation_Correct");
  tree_wrong = (TTree *)fin->Get("Permutation_Wrong");

  TString fout_name;
  if (chk_pre_cut)
    fout_name = Form("Vcb_PreCut_TTLJ_WtoCB_%dJets.root", n_jet);
  else if (chk_final_kin)
    fout_name = Form("Vcb_Permutation_TTLJ_WtoCB_%dJets.root", n_jet);
  else
    fout_name = Form("Vcb_PrePermutation_TTLJ_WtoCB_%dJets.root", n_jet);

  fout = TFile::Open(fout_name, "RECREATE");

  factory = new TMVA::Factory("TMVAClassification", fout,
                              "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");

  data_loader = new TMVA::DataLoader("dataset");
  // data_loader->AddVariable( "weight",       "weight",       "units", 'F');
  // data_loader->AddVariable( "n_jets",       "n_jets",       "units", 'I');
  data_loader->AddSpectator("n_jets", "n_jets", "units", 'I');

  // data_loader->AddVariable("n_bjets",       "n_bjets",       "units", 'I');
  // data_loader->AddVariable("n_cjets",       "n_cjets",       "units", 'I');

  data_loader->AddVariable("pt_had_t_b", "pt_had_t_b", "units", 'F');
  data_loader->AddVariable("pt_w_u", "pt_w_u", "units", 'F');
  data_loader->AddVariable("pt_w_d", "pt_w_d", "units", 'F');
  data_loader->AddVariable("pt_lep_t_b", "pt_lep_t_b", "units", 'F');

  // data_loader->AddVariable("eta_had_t_b", "eta_had_t_b", "units", 'F');
  // data_loader->AddVariable("eta_w_u", "eta_w_u", "units", 'F');
  // data_loader->AddVariable("eta_w_d", "eta_w_d", "units", 'F');
  // data_loader->AddVariable("eta_lep_t_b", "eta_lep_t_b", "units", 'F');

  data_loader->AddVariable("bvsc_had_t_b", "bvsc_had_t_b", "units", 'F');
  data_loader->AddVariable("cvsb_had_t_b", "cvsb_had_t_b", "units", 'F');
  data_loader->AddVariable("cvsl_had_t_b", "cvsl_had_t_b", "units", 'F');

  if (chk_final_kin)
  {
    data_loader->AddVariable("bvsc_w_u", "bvsc_w_u", "units", 'F');
    data_loader->AddVariable("cvsb_w_u", "cvsb_w_u", "units", 'F');
    data_loader->AddVariable("cvsl_w_u", "cvsl_w_u", "units", 'F');

    data_loader->AddVariable("bvsc_w_d", "bvsc_w_d", "units", 'F');
    data_loader->AddVariable("cvsb_w_d", "cvsb_w_d", "units", 'F');
    data_loader->AddVariable("cvsl_w_d", "cvsl_w_d", "units", 'F');
  }

  data_loader->AddVariable("bvsc_lep_t_b", "bvsc_lep_t_b", "units", 'F');
  data_loader->AddVariable("cvsb_lep_t_b", "cvsb_lep_t_b", "units", 'F');
  data_loader->AddVariable("cvsl_lep_t_b", "cvsl_lep_t_b", "units", 'F');

  // data_loader->AddVariable("del_phi_w_u_w_d", "del_phi_w_u_w_d", "units", 'F');
  // data_loader->AddVariable("del_eta_w_u_w_d", "del_eta_w_u_w_d", "units", 'F');
  // data_loader->AddVariable("del_r_w_u_w_d", "del_r_w_u_w_d", "units", 'F');
  data_loader->AddVariable("theta_w_u_w_d", "theta_w_u_w_d", "units", 'F');

  // data_loader->AddVariable("del_phi_had_w_had_t_b", "del_phi_had_w_had_t_b", "units", 'F');
  // data_loader->AddVariable("del_eta_had_w_had_t_b", "del_eta_had_w_had_t_b", "units", 'F');
  // data_loader->AddVariable("del_r_had_w_had_t_b", "del_r_had_w_had_t_b", "units", 'F');
  data_loader->AddVariable("theta_had_w_had_t_b", "theta_had_w_had_t_b", "units", 'F');

  // data_loader->AddVariable("del_phi_lep_neu", "del_phi_lep_neu", "units", 'F');
  // data_loader->AddVariable("del_eta_lep_neu", "del_eta_lep_neu", "units", 'F');
  // data_loader->AddVariable("del_r_lep_neu", "del_r_lep_neu", "units", 'F');
  data_loader->AddVariable("theta_lep_neu", "theta_lep_neu", "units", 'F');

  // data_loader->AddVariable("del_phi_lep_w_lep_t_b", "del_phi_lep_w_lep_t_b", "units", 'F');
  // data_loader->AddVariable("del_eta_lep_w_lep_t_b", "del_eta_lep_w_lep_t_b", "units", 'F');
  // data_loader->AddVariable("del_r_lep_w_lep_t_b", "del_r_lep_w_lep_t_b", "units", 'F');
  data_loader->AddVariable("theta_lep_w_lep_t_b", "theta_lep_w_lep_t_b", "units", 'F');

  // data_loader->AddVariable("del_phi_had_t_lep_t", "del_phi_had_t_lep_t", "units", 'F');
  // data_loader->AddVariable("del_eta_had_t_lep_t", "del_eta_had_t_lep_t", "units", 'F');
  // data_loader->AddVariable("del_r_had_t_lep_t", "del_r_had_t_lep_t", "units", 'F');
  // data_loader->AddVariable("theta_had_t_lep_t", "theta_had_t_lep_t", "units", 'F');

  data_loader->AddVariable("had_t_mass", "had_t_mass", "units", 'F');
  data_loader->AddVariable("had_w_mass", "had_w_mass", "units", 'F');
  data_loader->AddVariable("lep_t_mass", "lep_t_mass", "units", 'F');
  data_loader->AddVariable("lep_t_partial_mass", "lep_t_partial_mass", "units", 'F');

  if (!chk_pre_cut)
  {
    data_loader->AddVariable("chi2_jet_had_t_b", "chi2_jet_had_t_b", "units", 'F');
    data_loader->AddVariable("chi2_jet_w_u", "chi2_jet_w_u", "units", 'F');
    data_loader->AddVariable("chi2_jet_w_d", "chi2_jet_w_d", "units", 'F');
    data_loader->AddVariable("chi2_jet_lep_t_b", "chi2_jet_lep_t_b", "units", 'F');

    if (n_jet == 4)
      data_loader->AddSpectator("chi2_jet_extra", "chi2_jet_extra", "units", 'F');
    else
      data_loader->AddVariable("chi2_jet_extra", "chi2_jet_extra", "units", 'F');

    data_loader->AddVariable("chi2_constraint_had_t", "chi2_constraint_had_t", "units", 'F');
    data_loader->AddVariable("chi2_constraint_had_w", "chi2_constraint_had_w", "units", 'F');
    data_loader->AddVariable("chi2_constraint_lep_t", "chi2_constraint_lep_t", "units", 'F');
    data_loader->AddVariable("chi2_constraint_lep_w", "chi2_constraint_lep_w", "units", 'F');

    // data_loader->AddVariable("chi2",         "chi2",   "units", 'F');
  }

  data_loader->AddSignalTree(tree_correct, 1.0);
  data_loader->AddBackgroundTree(tree_wrong, 1.0);

  if (7 == n_jet)
  {
    cut_s = Form("%d<=n_jets&&0<n_matched_jets", n_jet);
    cut_b = Form("%d<=n_jets&&0<n_matched_jets", n_jet);
  }
  else
  {
    cut_s = Form("n_jets==%d", n_jet);
    cut_b = Form("n_jets==%d", n_jet);
  }

  n_train_signal = tree_correct->GetEntries(cut_s) / 2 / reduction;
  n_train_back = tree_wrong->GetEntries(cut_b) / 2 / reduction;

  // for debugging
  // n_train_signal = 100;
  // n_train_back = 100;

  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,
                                          Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:V", n_train_signal, n_train_back, n_train_signal, n_train_back));

  // //Adaptive Boost
  // factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDT",
  //                  "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");

  // Gradient Boost
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTG",
                      "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=100:MaxDepth=2");

  // Fisher
  // factory->BookMethod(data_loader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=100:NsmoothMVAPdf=10" );

  // DNN_CPU
  // General layout
  //  TString layoutString("Layout=RELU|128,RELU|128,RELU|128,LINEAR");

  // //Define Training strategy
  // TString trainingStrategyString = ("TrainingStrategy=LearningRate=1e-4,Momentum=0.9,"
  //                                "ConvergenceSteps=20,BatchSize=100,TestRepetitions=1,"
  //                                "WeightDecay=1e-4,Regularization=None,"
  //                                "DropConfig=0.0+0.5+0.5+0.5");

  // //General options
  // TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
  //                  "WeightInitialization=XAVIERUNIFORM");
  // dnnOptions.Append (":"); dnnOptions.Append (layoutString);
  // dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

  // TString cpuOptions = dnnOptions + ":Architecture=CPU";

  // factory->BookMethod(data_loader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
} // Permutation_TMVA::Permutation_TMVA()

//////////

Permutation_TMVA::~Permutation_TMVA()
{
  fout->Close();
} // Permutation_TMVA::~Permutation_TMVA()

//////////
