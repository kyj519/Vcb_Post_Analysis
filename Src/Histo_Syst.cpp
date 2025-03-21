#include "Histo_Syst.h"

ClassImp(Histo_Syst);

//////////

Histo_Syst::Histo_Syst(const TString &a_era, const TString &a_channel, const bool &a_chk_merge, const TString &a_run_flag, const bool &a_chk_template_on, const TString &a_swap_mode)
    : samples(a_era, a_channel), event(a_era, a_channel, a_swap_mode), tagging_rf(a_era)
{
  ROOT::EnableImplicitMT(5);

  cout << "[Histo_Syst::Histo_Syst]: Init analysis" << endl;

  reduction = 1000;

  era = a_era;
  channel = a_channel;
  chk_merge = a_chk_merge;
  run_flag = a_run_flag;
  chk_template_on = a_chk_template_on;

  if (era == "2016preVFP")
  {
    bvsc_wp_m = bvsc_wp_m_2016preVFP;
    cvsb_wp_m = cvsb_wp_m_2016preVFP;
    cvsl_wp_m = cvsl_wp_m_2016preVFP;
  }
  else if (era == "2016postVFP")
  {
    bvsc_wp_m = bvsc_wp_m_2016postVFP;
    cvsb_wp_m = cvsb_wp_m_2016postVFP;
    cvsl_wp_m = cvsl_wp_m_2016postVFP;
  }
  else if (era == "2017")
  {
    bvsc_wp_m = bvsc_wp_m_2017;
    cvsb_wp_m = cvsb_wp_m_2017;
    cvsl_wp_m = cvsl_wp_m_2017;
  }
  else if (era == "2018")
  {
    bvsc_wp_m = bvsc_wp_m_2018;
    cvsb_wp_m = cvsb_wp_m_2018;
    cvsl_wp_m = cvsl_wp_m_2018;
  }

  path_base = getenv("Vcb_Post_Analysis_WD");

  // Region
  // Signal shoudl be last
  region_name = {"Control0", "Signal"};
  n_region = region_name.size();

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    // cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB"));

  vec_short_name_mc.push_back("TTLJ_JJ_2");  // TTLJ, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_JJ_4");  // TTLJ, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_JJ_45"); // TTLJ, w->cb

  vec_short_name_mc.push_back("TTLJ_CC_2");  // TTLJ+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_CC_4");  // TTLJ+cc, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_CC_45"); // TTLJ+cc, w->cb

  vec_short_name_mc.push_back("TTLJ_BB_2");  // TTLJ+bb, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_BB_4");  // TTLJ+bb, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_BB_45"); // TTLJ+cc, w->cb

  vec_short_name_mc.push_back("TTLL_JJ"); // TTLL+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLL_CC"); // TTLL+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLL_BB"); // TTLL+bb, w->cd or w->cs

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  // cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  // Systs
  syst_name = {"Nominal",
               //"B_Tag_HF_Down", "B_Tag_HF_Up", "B_Tag_JES_Down", "B_Tag_JES_Up", "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up", "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up", "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up", "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up", "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up", "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up",
               "C_Tag_Extrap_Down", "C_Tag_Extrap_Up", "C_Tag_Interp_Down", "C_Tag_Interp_Up",
               //"C_Tag_LHE_Scale_MuF_Down", "C_Tag_LHE_Scale_MuF_Up", "C_Tag_LHE_Scale_MuR_Down", "C_Tag_LHE_Scale_MuR_Up", "C_Tag_PS_FSR_Fixed_Down", "C_Tag_PS_FSR_Fixed_Up", "C_Tag_PS_ISR_Fixed_Down", "C_Tag_PS_ISR_Fixed_Up", "C_Tag_PU_Down", "C_Tag_PU_Up",
               "C_Tag_Stat_Down", "C_Tag_Stat_Up", "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up", "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up", "C_Tag_XSec_Br_Unc_WJets_C_Down", "C_Tag_XSec_Br_Unc_WJets_C_Up",
               //"C_Tag_JER_Down", "C_Tag_JER_Up", "C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up",
               "Jet_En_Down", "Jet_En_Up",
               "Jet_Res_Down", "Jet_Res_Up",
               "UE_Down", "UE_Up",
               "PDF_Alternative", "PDF_As_Down", "PDF_As_Up",
               "Pileup_Down", "Pileup_Up",
               "PU_Jet_Veto_Down", "PU_Jet_Veto_Up",
               "Prefire_Down", "Prefire_Up",
               "MuF_Down", "MuF_Up",
               "MuR_Down", "MuR_Up",
               //"MuF_MuR_Down", "MuR_MuR_Up",
               "FSR_Down", "FSR_Up",
               "ISR_Down", "ISR_Up",
               "Top_Pt_Reweight",
               "Trig_Down", "Trig_Up",
               "CP5_Down", "CP5_Up",
               "hDamp_Down", "hDamp_Up",
               "mTop_171p5", "mTop_173p5"};

  if (channel == "Mu")
  {
    syst_name.push_back("Mu_Id_Down");
    syst_name.push_back("Mu_Id_Up");
    syst_name.push_back("Mu_Iso_Down");
    syst_name.push_back("Mu_Iso_Up");
  }
  else if (channel == "El")
  {
    syst_name.push_back("El_Id_Down");
    syst_name.push_back("El_Id_Up");
    syst_name.push_back("El_Reco_Down");
    syst_name.push_back("El_Reco_Up");
    //   syst_name.push_back("El_En_Down");
    //   syst_name.push_back("El_En_Up");
    //   syst_name.push_back("El_Res_Down");
    //   syst_name.push_back("El_Res_Up");
  }

  for (int i = 0; i < sizeof(event.weight_pdf_error_set) / sizeof(float); i++)
  {
    TString pdf_error_set = "PDF_Error_Set_" + to_string(i);
    syst_name.push_back(pdf_error_set);
  }

  n_syst = syst_name.size();
  cout << "N_Syst = " << n_syst << endl;

  // variable Config.
  variable_conf = {{"N_Vertex", 80, 0, 80},
                   {"Lepton_Pt", 50, 0, 300},
                   {"Lepton_Eta", 50, -3, 3},
                   {"N_Jets", 30, 0, 30},
                   {"N_BJets", 30, 0, 30},
                   {"N_CJets", 30, 0, 30},
                   {"Pt_Leading_Jet", 50, 0, 300},
                   {"Pt_Subleading_Jet", 50, 0, 300},
                   {"Eta_Leading_Jet", 50, -3, 3},
                   {"Eta_Subleading_jet", 50, -3, 3},
                   {"BvsC_Leading_Jet", 50, 0, 1},
                   {"CvsB_Leading_Jet", 50, 0, 1},
                   {"CvsL_Leading_Jet", 50, 0, 1},
                   {"BvsC_Subleading_Jet", 50, 0, 1},
                   {"CvsB_Subleading_Jet", 50, 0, 1},
                   {"CvsL_Subleading_Jet", 50, 0, 1},
                   {"Met_Pt", 50, 0, 300},
                   {"Met_Phi", 80, -4, 4},
                   {"Best_MVA_Score", 100, -1, 1}};
  //{"Had_W_Mass", 60, 60, 120},
  //{"Had_T_Mass", 80, 130, 210}};

  if (chk_template_on)
    variable_conf.push_back({"Template_MVA_Score", 100, 0, 1});

  n_variable = variable_conf.size();

  if (!chk_merge)
    Init_Histo_Syst();
  else
    Init_Merge_PDF_Error_Set();
} // Histo_Syst::Histo_Syst(const TString& a_era, const TString& a_channel, const TString& a_swap_mode)

//////////

Histo_Syst::~Histo_Syst()
{
  if (!chk_merge)
  {
    // Save histos
    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = fout->mkdir(region_name[i]);

      if ((run_flag == "Data" || run_flag == "All") && region_name[i] != "Signal")
      {
        // for Data
        TDirectory *dir_data = dir_region->mkdir("Data");
        for (int j = 0; j < n_variable; j++)
        {
          dir_data->cd();
          histo_data[i][j]->SetTitle(variable_conf[j].variable_title);
          histo_data[i][j]->SetName(variable_conf[j].variable_title);
          histo_data[i][j]->Write();
        } // loop over variable
      }

      // for MC
      for (int j = 0; j < n_syst; j++)
      {
        if (run_flag != "All" && run_flag != "Central" && run_flag != syst_name[j])
          continue;

        TDirectory *dir_syst = dir_region->mkdir(syst_name[j]);

        for (int k = 0; k < n_sample_merge_mc; k++)
        {
          TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[k]);

          for (int l = 0; l < n_variable; l++)
          {
            dir_sample->cd();
            histo_mc[i][j][k][l]->SetTitle(variable_conf[l].variable_title);
            histo_mc[i][j][k][l]->SetName(variable_conf[l].variable_title);
            histo_mc[i][j][k][l]->Write();
          } // loop over variable
        }   // loop over sample
      }     // loop over syst
    }       // loop over n_region
  }         // if(!chk_merge)
  // PDF error set
  else
  {
    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = (TDirectory *)fout->Get(region_name[i]);

      TDirectory *dir_syst = dir_region->mkdir("PDF_Error_Set_Down", "PDF_Error_Set_Down", true);
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[j], vec_short_name_mc[j], true);
        for (int k = 0; k < n_variable; k++)
        {
          dir_sample->cd();
          histo_mc_pdf_error_set_down[i][j][k]->SetTitle(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_down[i][j][k]->SetName(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_down[i][j][k]->Write();
        } // loop over n_variable
      }   // loop over n_sample_merge_mc;

      dir_syst = dir_region->mkdir("PDF_Error_Set_Up", "PDF_Error_Set_Up", true);
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[j], vec_short_name_mc[j], true);
        for (int k = 0; k < n_variable; k++)
        {
          dir_sample->cd();
          histo_mc_pdf_error_set_up[i][j][k]->SetTitle(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_up[i][j][k]->SetName(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_up[i][j][k]->Write();
        } // loop over n_variable
      }   // loop over n_sample_merge_mc;
    }     // loop over n_region
  }       // else

  fout->Close();
} // Histo_Syst::~Histo_Syst()

//////////

void Histo_Syst::Run()
{
  Read_Tree();

  return;
} // void Histo_Syst::Run()

//////////

void Histo_Syst::Run_Merge()
{
  Merge_PDF_Error_Set();

  return;
} // void Histo_Syst::Run_Merge()

//////////

void Histo_Syst::Fill_Histo_Data(const int &region_index)
{
  // this part is not satisfactory, but don't waste time
  histo_data[region_index][0]->Fill(event.n_pv, 1.);
  histo_data[region_index][1]->Fill(event.lepton_pt, 1.);
  histo_data[region_index][2]->Fill(event.lepton_eta, 1.);
  histo_data[region_index][3]->Fill(event.n_jets, 1.);
  histo_data[region_index][4]->Fill(event.n_bjets, 1.);
  histo_data[region_index][5]->Fill(event.n_cjets, 1.);
  histo_data[region_index][6]->Fill(event.pt_leading_jet, 1.);
  histo_data[region_index][7]->Fill(event.pt_subleading_jet, 1.);
  histo_data[region_index][8]->Fill(event.eta_leading_jet, 1.);
  histo_data[region_index][9]->Fill(event.eta_subleading_jet, 1.);
  histo_data[region_index][10]->Fill(event.bvsc_leading_jet, 1.);
  histo_data[region_index][11]->Fill(event.cvsb_leading_jet, 1.);
  histo_data[region_index][12]->Fill(event.cvsl_leading_jet, 1.);
  histo_data[region_index][13]->Fill(event.bvsc_subleading_jet, 1.);
  histo_data[region_index][14]->Fill(event.cvsb_subleading_jet, 1.);
  histo_data[region_index][15]->Fill(event.cvsl_subleading_jet, 1.);
  histo_data[region_index][16]->Fill(event.met_pt, 1.);
  histo_data[region_index][17]->Fill(event.met_phi, 1.);
  histo_data[region_index][18]->Fill(event.best_mva_score, 1.);
  // histo_data[region_index][19]->Fill(event.)

  if (chk_template_on)
  {
    histo_data[region_index][19]->Fill(event.template_score, 1);
  }

  return;
} // void Histo_Syst::Fill_Histo_Data()

//////////

void Histo_Syst::Fill_Histo_MC(const int &region_index, const TString &sample_name_short, const TString &syst_fix)
{
  cout << "test Fill_Histo_MC" << endl;

  //int sample_index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name_short) - vec_short_name_mc.begin();
  int histo_index = Histo_Index(sample_name_short);

  for (int i = 0; i < n_syst; i++)
  {
    TString syst_type = syst_name[i];

    // for central tree, don't consider the following syst
    if (syst_fix == "None" &&
        (syst_type == "Jet_En_Down" || syst_type == "Jet_En_Up" ||
         syst_type == "Jet_Res_Down" || syst_type == "Jet_Res_Up" ||
         syst_type == "El_En_Down" || syst_type == "El_En_Up" ||
         syst_type == "El_Res_Down" || syst_type == "El_Res_Up" ||
         syst_type == "UE_Down" || syst_type == "UE_Up" ||
         syst_type == "CP5_Down" || syst_type == "CP5_Up" ||
         syst_type == "hDamp_Down" || syst_type == "hDamp_Up" ||
         syst_type == "mTop_171p5" || syst_type == "mTop_173p5"))
      continue;

    // if syst_fix is fixed, only nominal is considered
    if (syst_fix != "None" && syst_fix != syst_type)
      continue;

    event.weight = 1;
    event.weight *= event.weight_lumi;
    event.weight *= event.weight_mc;
    event.weight *= event.weight_hem_veto;

    if (syst_type == "PDF_Alternative")
      event.weight *= event.weight_pdf_alternative;
    else if (syst_type.Contains("PDF_Error_Set_"))
    {
      TString temp = syst_type;

      int first = temp.First('_');
      temp = temp.Remove(0, first + 1);

      int second = temp.First('_');
      temp = temp.Remove(0, second + 1);

      int third = temp.First('_');
      TString index = temp.Remove(0, third + 1);

      event.weight *= event.weight_pdf_error_set[index.Atoi()];
    }
    else if (syst_type == "PDF_As_Down")
      event.weight *= event.weight_pdf_as_down;
    else if (syst_type == "PDF_As_Up")
      event.weight *= event.weight_pdf_as_up;

    // parton shower
    if (syst_type == "FSR_Down")
      event.weight *= event.weight_ps[0];
    else if (syst_type == "FSR_Up")
      event.weight *= event.weight_ps[1];
    else if (syst_type == "ISR_Down")
      event.weight *= event.weight_ps[2];
    else if (syst_type == "ISR_Up")
      event.weight *= event.weight_ps[3];

    // scale variation
    if (syst_type == "MuF_Down")
      event.weight *= event.weight_scale_variation_2;
    else if (syst_type == "MuF_Up")
      event.weight *= event.weight_scale_variation_1;
    else if (syst_type == "MuR_Down")
      event.weight *= event.weight_scale_variation_6;
    else if (syst_type == "MuR_Up")
      event.weight *= event.weight_scale_variation_3;
    else if (syst_type == "MuF_MuR_Down")
      event.weight *= event.weight_scale_variation_8;
    else if (syst_type == "MuF_MuR_Up")
      event.weight *= event.weight_scale_variation_4;

    if (syst_type == "Pileup_Down")
      event.weight *= event.weight_pileup_down;
    else if (syst_type == "Pileup_Up")
      event.weight *= event.weight_pileup_up;
    else
      event.weight *= event.weight_pileup;

    if (syst_type == "Prefire_Down")
      event.weight *= event.weight_prefire_down;
    else if (syst_type == "Prefire_Up")
      event.weight *= event.weight_prefire_up;
    else
      event.weight *= event.weight_prefire;

    // pileup jet veto id
    if (syst_type == "PU_Jet_Veto_Down")
      event.weight *= event.weight_pujet_veto_down;
    else if (syst_type == "PU_Jet_Veto_Up")
      event.weight *= event.weight_pujet_veto_up;
    else
      event.weight *= event.weight_pujet_veto;

    if (syst_type == "Top_Pt_Reweight")
      event.weight *= 1;
    else
      event.weight *= event.weight_top_pt;

    if (syst_type == "Trig_Down")
      event.weight *= event.weight_sl_trig_down;
    else if (syst_type == "Trig_Up")
      event.weight *= event.weight_sl_trig_up;
    else
      event.weight *= event.weight_sl_trig;

    if (syst_type == "Mu_Id_Down")
      event.weight *= event.weight_mu_id_down;
    else if (syst_type == "Mu_Id_Up")
      event.weight *= event.weight_mu_id_up;
    else
      event.weight *= event.weight_mu_id;

    if (syst_type == "Mu_Iso_Down")
      event.weight *= event.weight_mu_iso_down;
    else if (syst_type == "Mu_Iso_Up")
      event.weight *= event.weight_mu_iso_up;
    else
      event.weight *= event.weight_mu_iso;

    if (syst_type == "El_Id_Down")
      event.weight *= event.weight_el_id_down;
    else if (syst_type == "El_Id_Up")
      event.weight *= event.weight_el_id_up;
    else
      event.weight *= event.weight_el_id;

    if (syst_type == "El_Reco_Down")
      event.weight *= event.weight_el_reco_down;
    else if (syst_type == "El_Reco_Up")
      event.weight *= event.weight_el_reco_up;
    else
      event.weight *= event.weight_el_reco;

    // event.weight *= event.weight_pujet_veto;

    /*
        if (syst_type == "B_Tag_HF_Down")
          event.weight *= event.weight_b_tag_hf_down;
        else if (syst_type == "B_Tag_HF_Up")
          event.weight *= event.weight_b_tag_hf_up;
        else if (syst_type == "B_Tag_JES_Down")
          event.weight *= event.weight_b_tag_jes_down;
        else if (syst_type == "B_Tag_JES_Up")
          event.weight *= event.weight_b_tag_jes_up;
        else if (syst_type == "B_Tag_LFStats1_Down")
          event.weight *= event.weight_b_tag_lfstats1_down;
        else if (syst_type == "B_Tag_LFStats1_Up")
          event.weight *= event.weight_b_tag_lfstats1_up;
        else if (syst_type == "B_Tag_LFStats2_Down")
          event.weight *= event.weight_b_tag_lfstats2_down;
        else if (syst_type == "B_Tag_LFStats2_Up")
          event.weight *= event.weight_b_tag_lfstats2_up;
        else if (syst_type == "B_Tag_CFErr1_Down")
          event.weight *= event.weight_b_tag_cferr1_down;
        else if (syst_type == "B_Tag_CFErr1_Up")
          event.weight *= event.weight_b_tag_cferr1_up;
        else if (syst_type == "B_Tag_CFErr2_Down")
          event.weight *= event.weight_b_tag_cferr2_down;
        else if (syst_type == "B_Tag_CFErr2_Up")
          event.weight *= event.weight_b_tag_cferr2_up;
        else if (syst_type == "B_Tag_HFStats1_Down")
          event.weight *= event.weight_b_tag_hfstats1_down;
        else if (syst_type == "B_Tag_HFStats1_Up")
          event.weight *= event.weight_b_tag_hfstats1_up;
        else if (syst_type == "B_Tag_HFStats2_Down")
          event.weight *= event.weight_b_tag_hfstats2_down;
        else if (syst_type == "B_Tag_HFStats2_Up")
          event.weight *= event.weight_b_tag_hfstats2_up;
        else
          event.weight *= event.weight_b_tag;

       // b-tag renormalization factor
    b_tag_rf = 1;
    // if (syst_type.Contains("B_Tag"))
    //   b_tag_rf = tagging_rf.Get_Tagging_RF(syst_type, event.n_jets);
    // else
    //   b_tag_rf = tagging_rf.Get_Tagging_RF("B_Tag_Nominal", event.n_jets);

    event.weight *= b_tag_rf;
        */

    TString c_tagging_rf_type;
    if (syst_type == "C_Tag_Extrap_Down")
    {
      event.weight *= event.weight_c_tag_extrap_down;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_Extrap_Up")
    {
      event.weight *= event.weight_c_tag_extrap_up;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_Interp_Down")
    {
      event.weight *= event.weight_c_tag_interp_down;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_Interp_Up")
    {
      event.weight *= event.weight_c_tag_interp_up;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_Stat_Down")
    {
      event.weight *= event.weight_c_tag_stat_down;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_Stat_Up")
    {
      event.weight *= event.weight_c_tag_stat_up;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_B_Down")
    {
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_b_down;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_B_Up")
    {
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_b_up;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_C_Down")
    {
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_c_down;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_C_Up")
    {
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_c_up;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_XSec_Br_Unc_WJets_C_Down")
    {
      event.weight *= event.weight_c_tag_xsec_br_unc_wjets_c_down;
      c_tagging_rf_type = syst_type;
    }
    else if (syst_type == "C_Tag_XSec_Br_Unc_WJets_C_Up")
    {
      event.weight *= event.weight_c_tag_xsec_br_unc_wjets_c_up;
      c_tagging_rf_type = syst_type;
    }
    // fully correlated c-tagging scale factors
    else if (syst_type == "Jet_Res_Down")
    {
      event.weight *= event.weight_c_tag_jer_down;
      c_tagging_rf_type = "C_Tag_JER_Down";
    }
    else if (syst_type == "Jet_Res_Up")
    {
      event.weight *= event.weight_c_tag_jer_up;
      c_tagging_rf_type = "C_Tag_JER_Up";
    }
    else if (syst_type == "Jet_En_Down")
    {
      event.weight *= event.weight_c_tag_jes_total_down;
      c_tagging_rf_type = "C_Tag_JES_Total_Down";
    }
    else if (syst_type == "Jet_En_Up")
    {
      event.weight *= event.weight_c_tag_jes_total_up;
      c_tagging_rf_type = "C_Tag_JES_Total_Up";
    }
    else if (syst_type == "MuF_Down")
    {
      event.weight *= event.weight_c_tag_lhe_scale_muf_down;
      c_tagging_rf_type = "C_Tag_LHE_Scale_MuF_Down";
    }
    else if (syst_type == "MuF_Up")
    {
      event.weight *= event.weight_c_tag_lhe_scale_muf_up;
      c_tagging_rf_type = "C_Tag_LHE_Scale_MuF_Up";
    }
    else if (syst_type == "MuR_Down")
    {
      event.weight *= event.weight_c_tag_lhe_scale_mur_down;
      c_tagging_rf_type = "C_Tag_LHE_Scale_MuR_Down";
    }
    else if (syst_type == "MuR_Up")
    {
      event.weight *= event.weight_c_tag_lhe_scale_mur_up;
      c_tagging_rf_type = "C_Tag_LHE_Scale_MuR_Up";
    }
    else if (syst_type == "FSR_Down")
    {
      event.weight *= event.weight_c_tag_ps_fsr_fixed_down;
      c_tagging_rf_type = "C_Tag_PS_FSR_Fixed_Down";
    }
    else if (syst_type == "FSR_Up")
    {
      event.weight *= event.weight_c_tag_ps_fsr_fixed_up;
      c_tagging_rf_type = "C_Tag_PS_FSR_Fixed_Up";
    }
    else if (syst_type == "ISR_Down")
    {
      event.weight *= event.weight_c_tag_ps_isr_fixed_down;
      c_tagging_rf_type = "C_Tag_PS_ISR_Fixed_Down";
    }
    else if (syst_type == "ISR_Up")
    {
      event.weight *= event.weight_c_tag_ps_isr_fixed_up;
      c_tagging_rf_type = "C_Tag_PS_ISR_Fixed_Up";
    }
    else if (syst_type == "Pileup_Down")
    {
      event.weight *= event.weight_c_tag_pu_down;
      c_tagging_rf_type = "C_Tag_PU_Down";
    }
    else if (syst_type == "Pileup_Up")
    {
      event.weight *= event.weight_c_tag_pu_up;
      c_tagging_rf_type = "C_Tag_PU_Up";
    }
    else
    {
      event.weight *= event.weight_c_tag;
      c_tagging_rf_type = "C_Tag_Nominal";
    }

    event.weight *= tagging_rf.Get_Tagging_RF_C_Tag(vec_short_name_mc[histo_index], c_tagging_rf_type, event.n_pv, event.ht);

    // this part is not satisfactory, but don't waste time
    histo_mc[region_index][i][histo_index][0]->Fill(event.n_pv, event.weight);
    histo_mc[region_index][i][histo_index][1]->Fill(event.lepton_pt, event.weight);
    histo_mc[region_index][i][histo_index][2]->Fill(event.lepton_eta, event.weight);
    histo_mc[region_index][i][histo_index][3]->Fill(event.n_jets, event.weight);
    histo_mc[region_index][i][histo_index][4]->Fill(event.n_bjets, event.weight);
    histo_mc[region_index][i][histo_index][5]->Fill(event.n_cjets, event.weight);
    histo_mc[region_index][i][histo_index][6]->Fill(event.pt_leading_jet, event.weight);
    histo_mc[region_index][i][histo_index][7]->Fill(event.pt_subleading_jet, event.weight);
    histo_mc[region_index][i][histo_index][8]->Fill(event.eta_leading_jet, event.weight);
    histo_mc[region_index][i][histo_index][9]->Fill(event.eta_subleading_jet, event.weight);
    histo_mc[region_index][i][histo_index][10]->Fill(event.bvsc_leading_jet, event.weight);
    histo_mc[region_index][i][histo_index][11]->Fill(event.cvsb_leading_jet, event.weight);
    histo_mc[region_index][i][histo_index][12]->Fill(event.cvsl_leading_jet, event.weight);
    histo_mc[region_index][i][histo_index][13]->Fill(event.bvsc_subleading_jet, event.weight);
    histo_mc[region_index][i][histo_index][14]->Fill(event.cvsb_subleading_jet, event.weight);
    histo_mc[region_index][i][histo_index][15]->Fill(event.cvsl_subleading_jet, event.weight);
    histo_mc[region_index][i][histo_index][16]->Fill(event.met_pt, event.weight);
    histo_mc[region_index][i][histo_index][17]->Fill(event.met_phi, event.weight);
    histo_mc[region_index][i][histo_index][18]->Fill(event.best_mva_score, event.weight);

    if (chk_template_on)
      histo_mc[region_index][i][histo_index][19]->Fill(event.template_score, event.weight);
  } // loop over n_syst

  return;
} // void Histo_Syst::Fill_Histo_MC(const int& sample_index)

//////////

int Histo_Syst::Histo_Index(const TString& sample_name)
{
  int index = -999;

  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")))
  {
    bool chk_b = false;
    bool chk_c = false;

    // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    {
      int flavour = event.vec_sel_gen_hf_flavour->at(i);
      int origin = event.vec_sel_gen_hf_origin->at(i);

      if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      {
        chk_b = true;
        break;
      }
      else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
        chk_c = true;
    }

    TString histo_name = samples.map_short_name_mc[sample_name];
    if (histo_name.Contains("WtoCB"))
      histo_name = "TTLJ";

    if (chk_b)
      histo_name += "_BB";
    else if (chk_c)
      histo_name += "_CC";
    else
      histo_name += "_JJ";

    if (event.decay_mode == 21 || event.decay_mode == 23)
      histo_name += "_2";
    else if (event.decay_mode == 41 || event.decay_mode == 43)
      histo_name += "_4";
    else if (event.decay_mode == 45)
      histo_name += "_45";

    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name]) - vec_short_name_mc.begin();

  // cout << "test Histo_Index: " << sample_name << " " << index << endl;

  return index;
} // int Histo_Syst::Histo_Index(const TString& sample_name)

//////////

int Histo_Syst::Get_Region_Index(const TString &region)
{
  return find(region_name.begin(), region_name.end(), region) - region_name.begin();
} // Region_Index Histo_Syst::Get_Region_Index(const TString& region)

//////////

void Histo_Syst::Init_Histo_Syst()
{
  cout << "[Histo_Syst::Init_Histo_Syst]: Init" << endl;

  // job splitting for central and syst_reweigt
  if (run_flag.Contains("Central"))
  {
    index_split = TString(run_flag[7]).Atoi();
    n_split = TString(run_flag[8]).Atoi();
    run_flag = "Central";
  }

  cout << "[Histo_Syst::Histo_Syst]: run_flag = " << run_flag << endl;
  if (run_flag == "Central")
    cout << "Index_Split = " << index_split << ", N_Split = " << n_split << endl;

  if (channel == "Mu" && run_flag.Contains("Electron"))
    throw "Input run_flag is not compatible with Mu channel. Abort process.";

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  /*
  if (channel == "Mu")
    // key_base = "POGTightWithTightIso_";
    key_base = "";
  else if (channel = "El")
    // key_base = "passTightID_";
    // key_base = "passMVAID_iso_WP80_";
    key_base = "";
  */

  tree_name = "/Result_Tree";

  if (run_flag != "Data")
  {
    for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
    {
      cout << it->first << endl;

      map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);

      if (run_flag == "Central" ||
          run_flag == "CP5_Down" || run_flag == "CP5_Up" ||
          run_flag == "hDamp_Down" || run_flag == "hDamp_Up" ||
          run_flag == "mTop_171p5" || run_flag == "mTop_173p5" ||
          run_flag == "All")
      {
        map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "Central" + tree_name);
        cout << "test 0" << endl;
        event.Setup_Tree(map_tree_mc[it->first], Syst::Central, true);
        cout << "test 1" << endl;
      }

      if (run_flag == "Jet_En_Down" || run_flag == "All")
      {
        cout << "test 10" << endl;
        map_tree_mc_jec_down[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetEnDown" + tree_name);
        cout << "test 11" << endl;
        event.Setup_Tree(map_tree_mc_jec_down[it->first], Syst::JetEnDown, false);
        cout << "test 2" << endl;
      }

      if (run_flag == "Jet_En_Up" || run_flag == "All")
      {
        map_tree_mc_jec_up[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetEnUp" + tree_name);
        event.Setup_Tree(map_tree_mc_jec_up[it->first], Syst::JetEnUp, false);
      }

      if (run_flag == "Jet_Res_Down" || run_flag == "All")
      {
        map_tree_mc_jer_down[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetResDown" + tree_name);
        event.Setup_Tree(map_tree_mc_jer_down[it->first], Syst::JetResDown, false);
      }

      if (run_flag == "Jet_Res_Up" || run_flag == "All")
      {
        map_tree_mc_jer_up[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetResUp" + tree_name);
        event.Setup_Tree(map_tree_mc_jer_up[it->first], Syst::JetResUp, false);
      }

      if (run_flag == "UE_Down" || run_flag == "All")
      {
        map_tree_mc_ue_down[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "UEDown" + tree_name);
        event.Setup_Tree(map_tree_mc_ue_down[it->first], Syst::Central, false);
      }

      if (run_flag == "UE_Up" || run_flag == "All")
      {
        map_tree_mc_ue_up[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "UEUp" + tree_name);
        event.Setup_Tree(map_tree_mc_ue_up[it->first], Syst::Central, false);
        cout << "test 3" << endl;
      }

      /*
      if (channel == "El")
      {
        if (run_flag == "El_En_Down" || run_flag == "All")
        {
          map_fin_mc_eec_down[it->first] = new TFile(result_path + "ElectronEnDown/" + it->second);
          map_tree_mc_eec_down[it->first] = (TTree *)map_fin_mc_eec_down[it->first]->Get(key_base + "ElectronEnDown" + tree_name);

          event.Setup_Tree(map_tree_mc_eec_down[it->first], Syst::ElectronEnDown, false);
        }

        if (run_flag == "El_En_Up" || run_flag == "All")
        {
          map_fin_mc_eec_up[it->first] = new TFile(result_path + "ElectronEnUp/" + it->second);
          map_tree_mc_eec_up[it->first] = (TTree *)map_fin_mc_eec_up[it->first]->Get(key_base + "ElectronEnUp" + tree_name);

          event.Setup_Tree(map_tree_mc_eec_up[it->first], Syst::ElectronEnUp, false);
        }

        if (run_flag == "El_Res_Down")
        {
          map_fin_mc_eer_down[it->first] = new TFile(result_path + "ElectronResDown/" + it->second);
          map_tree_mc_eer_down[it->first] = (TTree *)map_fin_mc_eer_down[it->first]->Get(key_base + "ElectronResDown" + tree_name);

          event.Setup_Tree(map_tree_mc_eer_down[it->first], Syst::ElectronResDown, false);
        }

        if (run_flag == "El_Res_Up" || run_flag == "All")
        {
          map_fin_mc_eer_up[it->first] = new TFile(result_path + "ElectronResUp/" + it->second);
          map_tree_mc_eer_up[it->first] = (TTree *)map_fin_mc_eer_up[it->first]->Get(key_base + "ElectronResUp" + tree_name);

          event.Setup_Tree(map_tree_mc_eer_up[it->first], Syst::ElectronResUp, false);
        }
      }
      */
    }

    if (run_flag == "CP5_Down" || run_flag == "All")
    {
      map_fin_mc_cp5_down = map_fin_mc;
      map_tree_mc_cp5_down = map_tree_mc;

      map_fin_mc_cp5_down["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_CP5down"]);
      map_tree_mc_cp5_down["TTLJ_powheg"] = (TTree *)map_fin_mc_cp5_down["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_cp5_down["TTLJ_powheg"], Syst::Central, false);

      map_fin_mc_cp5_down["TTLL_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_powheg_CP5down"]);
      map_tree_mc_cp5_down["TTLL_powheg"] = (TTree *)map_fin_mc_cp5_down["TTLL_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_cp5_down["TTLL_powheg"], Syst::Central, false);
    }

    if (run_flag == "CP5_Up" || run_flag == "All")
    {
      map_fin_mc_cp5_up = map_fin_mc;
      map_tree_mc_cp5_up = map_tree_mc;

      map_fin_mc_cp5_up["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_CP5up"]);
      map_tree_mc_cp5_up["TTLJ_powheg"] = (TTree *)map_fin_mc_cp5_up["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_cp5_up["TTLJ_powheg"], Syst::Central, false);

      map_fin_mc_cp5_up["TTLL_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_powheg_CP5up"]);
      map_tree_mc_cp5_up["TTLL_powheg"] = (TTree *)map_fin_mc_cp5_up["TTLL_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_cp5_up["TTLL_powheg"], Syst::Central, false);
    }

    if (run_flag == "hDamp_Down" || run_flag == "All")
    {
      map_fin_mc_hdamp_down = map_fin_mc;
      map_tree_mc_hdamp_down = map_tree_mc;

      map_fin_mc_hdamp_down["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_hdampdown"]);
      map_tree_mc_hdamp_down["TTLJ_powheg"] = (TTree *)map_fin_mc_hdamp_down["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_hdamp_down["TTLJ_powheg"], Syst::Central, false);

      map_fin_mc_hdamp_down["TTLL_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_powheg_hdampdown"]);
      map_tree_mc_hdamp_down["TTLL_powheg"] = (TTree *)map_fin_mc_hdamp_down["TTLL_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_hdamp_down["TTLL_powheg"], Syst::Central, false);

      cout << "test 4" << endl;
    }

    if (run_flag == "hDamp_Up" || run_flag == "All")
    {
      map_fin_mc_hdamp_up = map_fin_mc;
      map_tree_mc_hdamp_up = map_tree_mc;

      map_fin_mc_hdamp_up["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_hdampup"]);
      map_tree_mc_hdamp_up["TTLJ_powheg"] = (TTree *)map_fin_mc_hdamp_up["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_hdamp_up["TTLJ_powheg"], Syst::Central, false);

      map_fin_mc_hdamp_up["TTLL_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_powheg_hdampup"]);
      map_tree_mc_hdamp_up["TTLL_powheg"] = (TTree *)map_fin_mc_hdamp_up["TTLL_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_hdamp_up["TTLL_powheg"], Syst::Central, false);
    }

    if (run_flag == "mTop_171p5" || run_flag == "All")
    {
      map_fin_mc_mtop_171p5 = map_fin_mc;
      map_tree_mc_mtop_171p5 = map_tree_mc;

      map_fin_mc_mtop_171p5["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_mtop_171p5"]);
      map_tree_mc_mtop_171p5["TTLJ_powheg"] = (TTree *)map_fin_mc_mtop_171p5["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_mtop_171p5["TTLJ_powheg"], Syst::Central, false);

      map_fin_mc_mtop_171p5["TTLL_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_powheg_mtop_171p5"]);
      map_tree_mc_mtop_171p5["TTLL_powheg"] = (TTree *)map_fin_mc_mtop_171p5["TTLL_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_mtop_171p5["TTLL_powheg"], Syst::Central, false);
    }

    if (run_flag == "mTop_173p5" || run_flag == "All")
    {
      map_fin_mc_mtop_173p5 = map_fin_mc;
      map_tree_mc_mtop_173p5 = map_tree_mc;

      map_fin_mc_mtop_173p5["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_mtop_173p5"]);
      map_tree_mc_mtop_173p5["TTLJ_powheg"] = (TTree *)map_fin_mc_mtop_173p5["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_mtop_173p5["TTLJ_powheg"], Syst::Central, false);

      map_fin_mc_mtop_173p5["TTLL_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_powheg_mtop_173p5"]);
      map_tree_mc_mtop_173p5["TTLL_powheg"] = (TTree *)map_fin_mc_mtop_173p5["TTLL_powheg"]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_mc_mtop_173p5["TTLL_powheg"], Syst::Central, false);
    }

    // TFile and TTree handlers
    if (run_flag == "Central" || run_flag == "All")
      map_map_tree_mc["Central"] = &map_tree_mc;

    if (run_flag == "Jet_En_Down" || run_flag == "All")
      map_map_tree_mc["Jet_En_Down"] = &map_tree_mc_jec_down;

    if (run_flag == "Jet_En_Up" || run_flag == "All")
      map_map_tree_mc["Jet_En_Up"] = &map_tree_mc_jec_up;

    if (run_flag == "Jet_Res_Down" || run_flag == "All")
      map_map_tree_mc["Jet_Res_Down"] = &map_tree_mc_jer_down;

    if (run_flag == "Jet_Res_Up" || run_flag == "All")
      map_map_tree_mc["Jet_Res_Up"] = &map_tree_mc_jer_up;

    if (run_flag == "UE_Down" || run_flag == "All")
      map_map_tree_mc["UE_Down"] = &map_tree_mc_ue_down;

    if (run_flag == "UE_Up" || run_flag == "All")
      map_map_tree_mc["UE_Up"] = &map_tree_mc_ue_up;

    if (run_flag == "CP5_Down" || run_flag == "All")
      map_map_tree_mc["CP5_Down"] = &map_tree_mc_cp5_down;

    if (run_flag == "CP5_Up" || run_flag == "All")
      map_map_tree_mc["CP5_Up"] = &map_tree_mc_cp5_up;

    if (run_flag == "hDamp_Down" || run_flag == "All")
      map_map_tree_mc["hDamp_Down"] = &map_tree_mc_hdamp_down;

    if (run_flag == "hDamp_Up" || run_flag == "All")
      map_map_tree_mc["hDamp_Up"] = &map_tree_mc_hdamp_up;

    if (run_flag == "mTop_171p5" || run_flag == "All")
      map_map_tree_mc["mTop_171p5"] = &map_tree_mc_mtop_171p5;

    if (run_flag == "mTop_173p5" || run_flag == "All")
      map_map_tree_mc["mTop_173p5"] = &map_tree_mc_mtop_173p5;

    /*
    if (channel == "El")
    {
      if (run_flag == "El_En_Down" || run_flag == "All")
      {
        map_map_fin_mc["El_En_Down"] = &map_fin_mc_eec_down;
        map_map_tree_mc["El_En_Down"] = &map_tree_mc_eec_down;
      }

      if (run_flag == "El_En_Up" || run_flag == "All")
      {
        map_map_fin_mc["El_En_Up"] = &map_fin_mc_eec_up;
        map_map_tree_mc["El_En_Up"] = &map_tree_mc_eec_up;
      }

      if (run_flag == "El_Res_Down" || run_flag == "All")
      {
        map_map_fin_mc["El_Res_Down"] = &map_fin_mc_eer_down;
        map_map_tree_mc["El_Res_Down"] = &map_tree_mc_eer_down;
      }

      if (run_flag == "El_Res_Up" || run_flag == "All")
      {
        map_map_fin_mc["El_Res_Up"] = &map_fin_mc_eer_up;
        map_map_tree_mc["El_Res_Up"] = &map_tree_mc_eer_up;
      }
    } // if (channel == "El")
    */
    cout << "test 5" << endl;
  } // if (run_flag != "Data")

  // data
  if (run_flag == "Data" || run_flag == "All")
  {
    result_path += "DATA/";
    for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
    {
      cout << it->first << endl;
      if (it == samples.map_data.begin())
        data_short_name = samples.map_short_name_data[it->first];

      map_fin_data[it->first] = new TFile(result_path + it->second);
      map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_data[it->first], Syst::Central, false);
    }
  } // if (run_flag == "Data" || run_flag == "All")

  // histo for MC
  histo_mc = new TH1D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc[i] = new TH1D ***[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      histo_mc[i][j] = new TH1D **[n_sample_merge_mc];
      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        histo_mc[i][j][k] = new TH1D *[n_variable];
        for (int l = 0; l < n_variable; l++)
        {
          TString histo_name = region_name[i] + "_" + syst_name[j] + "_" + vec_short_name_mc[k] + "_" + variable_conf[l].variable_title;
          histo_mc[i][j][k][l] = new TH1D(histo_name, variable_conf[l].variable_title, variable_conf[l].n_bin, variable_conf[l].x_low, variable_conf[l].x_up);
        } // loop over n_variable
      }   // loop over n_sample_merge_mc
    }     // loop over n_syst
  }       // loop over n_region

  // histo for data
  // n_region-1 to exclude signal region
  histo_data = new TH1D **[n_region - 1];
  for (int i = 0; i < n_region - 1; i++)
  {
    histo_data[i] = new TH1D *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = region_name[i] + "_" + variable_conf[j].variable_title;
      histo_data[i][j] = new TH1D(histo_name, variable_conf[j].variable_title, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up);
    } // loop over n_variable
  }   // loop over n_region

  TString fout_name = "Vcb_Histos_" + era + "_" + channel + "_";
  if (run_flag == "Central")
    fout_name += run_flag + to_string(index_split) + to_string(n_split);
  else
    fout_name += run_flag;
  fout_name += ".root";
  fout = new TFile(fout_name, "RECREATE");

  cout << "[Histo_Syst::Init_Histo_Syst]: Done" << endl;

  return;
} // void Histo_Syst::Init_Histo_Syst()

//////////

void Histo_Syst::Init_Merge_PDF_Error_Set()
{
  cout << "Histo_Syst::Init_Merge_PDF_Error_Set" << endl;

  TString fout_name = "Vcb_Histos_" + era + "_" + channel + "_All.root";
  fout = new TFile(fout_name, "UPDATE");

  // to check Template_MVA_Score is included
  TList *list_variable = ((TDirectory *)fout->Get("Control0/Nominal/TTLJ"))->GetListOfKeys();
  TObject *obj = list_variable->FindObject("Template_MVA_Score");
  if (obj != NULL)
  {
    variable_conf.push_back({"Template_MVA_Score", 100, 0, 1});
    n_variable = variable_conf.size();
  }

  histo_mc = new TH1D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc[i] = new TH1D ***[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      histo_mc[i][j] = new TH1D **[n_sample_merge_mc];
      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        histo_mc[i][j][k] = new TH1D *[n_variable];
        for (int l = 0; l < n_variable; l++)
        {
          TString histo_name = region_name[i] + "/" + syst_name[j] + "/" + vec_short_name_mc[k] + "/" + variable_conf[l].variable_title;
          histo_mc[i][j][k][l] = (TH1D *)fout->Get(histo_name);
        } // loop over n_variable
      }   // loop over n_sample_merge_mc
    }     // loop over n_syst
  }       // loop over n_region

  return;
} // void Histo_Syst::Init_Merge_PDF_Error_Set()

//////////

void Histo_Syst::Merge_PDF_Error_Set()
{
  cout << "Merging histograms for 100 pdf_error_sets" << endl;
  chk_merge_pdf_error_set = true;

  histo_mc_pdf_error_set_down = new TH1D ***[n_region];
  histo_mc_pdf_error_set_up = new TH1D ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc_pdf_error_set_down[i] = new TH1D **[n_sample_merge_mc];
    histo_mc_pdf_error_set_up[i] = new TH1D **[n_sample_merge_mc];
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      histo_mc_pdf_error_set_down[i][j] = new TH1D *[n_variable];
      histo_mc_pdf_error_set_up[i][j] = new TH1D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name_pdf_error = "PDF_Error_Set_Down_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        histo_mc_pdf_error_set_down[i][j][k] = new TH1D(histo_name_pdf_error, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);

        histo_name_pdf_error = "PDF_Error_Set_Up_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        histo_mc_pdf_error_set_up[i][j][k] = new TH1D(histo_name_pdf_error, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);
      } // loop over n_variable
    }   // loop over n_sample_merge_mc
  }     // loop over n_region

  // find min and max and fill histo
  for (int i = 0; i < n_region; i++)
  {
    cout << region_name[i] << endl;
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      cout << vec_short_name_mc[j] << endl;
      for (int k = 0; k < n_variable; k++)
      {
        for (int l = 0; l < variable_conf[k].n_bin; l++)
        {
          float max = -999999999999999999;
          float min = 999999999999999999;
          for (int m = 0; m < n_pdf_error_set; m++)
          {
            int index;
            auto it = find(syst_name.begin(), syst_name.end(), "PDF_Error_Set_" + to_string(m));
            if (it != syst_name.end())
              index = distance(syst_name.begin(), it);
            else
            {
              cout << "Can't find the corresponding PDF_Error_Set. Check it." << endl;
              exit(1);
            }

            float content = histo_mc[i][index][j][k]->GetBinContent(l + 1);

            // if(variable_conf[k].variable_title=="N_BJets" && content!=0) cout << content << " " << min << " " << max << endl;

            if (content < min)
              min = content;
            if (max < content)
              max = content;

          } // loop over n_pdf_error_set

          histo_mc_pdf_error_set_down[i][j][k]->SetBinContent(l + 1, min);
          histo_mc_pdf_error_set_up[i][j][k]->SetBinContent(l + 1, max);
        } // loop over n_bin

        histo_mc_pdf_error_set_down[i][j][k]->Sumw2();
        histo_mc_pdf_error_set_up[i][j][k]->Sumw2();
      } // loop over n_variable
    }   // loop over n_sample_merge_mc
  }     // loop over n_region

  return;
} // void Histo_Syst::Merge_PDF_Error_Set()

//////////

void Histo_Syst::Read_Tree()
{
  cout << "[Histo_Syst::Read_Tree]: Start to read trees" << endl;

  // for MC
  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString fin_name = it_map_map->first;
    cout << fin_name << endl;

    TString syst_fix;
    if (fin_name == "Central")
      syst_fix = "None";
    else
      syst_fix = it_map_map->first;

    map<TString, TTree *> *map_tree = it_map_map->second;

    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
      cout << it->first << endl;
      //TString sample_name_short = samples.map_short_name_mc[it->first];
      TString sample_name_short = samples.map_short_name_mc[it->first];
      // cout << sample_name_short << endl;

      // int sample_index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[it->first]) - vec_short_name_mc.begin();
      //  cout << it->first << " Sample_Index = " << sample_index << endl;

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      // split
      Long64_t init;
      Long64_t end;

      if (run_flag == "Central")
      {
        Long64_t step = n_entries / n_split;
        init = step * index_split;
        end = step * (index_split + 1);
        if (index_split + 1 == n_split)
          end = n_entries;

        cout << "N_Entries:" << n_entries << " N_Split:" << n_split << " Index_Split:" << index_split << " Step:" << step << " Init:" << init << " End:" << end << endl;
        cout << endl;
      }
      else
      {
        init = 0;
        end = n_entries;
      }

      for (Long64_t i = init; i < end; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        // event veto
        if (100 < TMath::Abs(event.weight_pdf_alternative))
        {
          cout << "strange weight_pdf_alternative is detected. " << fin_name << ", " << i << ", weight_pdf_alternative = " << event.weight_pdf_alternative << endl;
          cout << "Continue" << endl;

          continue;
        }

        event.Swap();

        TString region = Set_Region();

        if (region == "Out")
          continue;

        int region_index = Get_Region_Index(region);

        Fill_Histo_MC(region_index, sample_name_short, syst_fix);
        // if (syst_fix == "None") Fill_Histo_Weight(region_index, sample_index);
      } // loop over entries
    }   // loop over map_tree_mc

    // close fin to save memory
    // map<TString, TFile *> *map_fin = map_map_fin_mc[fin_name];
    // map_fin->at(fin_name)->Close();
  } // loop over map_map_tree_mc

  // for data
  for (auto it = map_tree_data.begin(); it != map_tree_data.end(); it++)
  {
    TString fin_name = it->first;
    cout << fin_name << endl;

    Long64_t n_entries = it->second->GetEntries();
    n_entries /= reduction;
    cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

    for (Long64_t i = 0; i < n_entries; i++)
    {
      if (i % 500000 == 0)
        cout << "Processing... " << i << "/" << n_entries << "." << endl;

      it->second->GetEntry(i);

      event.Swap();

      TString region = Set_Region();

      if (region == "Out" || region == "Signal")
        continue;

      int region_index = Get_Region_Index(region);

      Fill_Histo_Data(region_index);
    }

    // map_fin_data[fin_name]->Close();
  } // loop over data period

  return;
} // void Histo_Syst::Read_Tree()

//////////

void Histo_Syst::Setup_Template_Reader()
{
  reader_template = new TMVA::Reader("!Color:!Silent");
  reader_template->AddVariable("best_mva_score", &event.best_mva_score);
  reader_template->AddVariable("n_bjets", &n_bjets_f);
  reader_template->AddVariable("n_cjets", &n_cjets_f);
  reader_template->AddVariable("bvsc_w_u", &event.bvsc_w_u);
  reader_template->AddVariable("cvsb_w_u", &event.cvsb_w_u);
  reader_template->AddVariable("cvsl_w_u", &event.cvsl_w_u);
  reader_template->AddVariable("bvsc_w_d", &event.bvsc_w_d);
  reader_template->AddVariable("cvsb_w_d", &event.cvsb_w_d);
  reader_template->AddVariable("cvsl_w_d", &event.cvsl_w_d);

  template_mva_name = "Classification_BDTB";

  TString weight_file = getenv("Vcb_Post_Analysis_WD");
  weight_file += "/Macro/Template/";
  weight_file += era;
  weight_file += "/";
  weight_file += channel;
  weight_file += "/weights/TMVA";
  weight_file += template_mva_name;
  weight_file += ".weights.xml";

  reader_template->BookMVA(template_mva_name, weight_file);

  return;
} // void Histo_Syst::Setup_Template_Reader()

//////////

TString Histo_Syst::Set_Region()
{
  bool chk_b_tagged_had_t_b = bvsc_wp_m < event.bvsc_had_t_b ? true : false;
  bool chk_b_tagged_lep_t_b = bvsc_wp_m < event.bvsc_lep_t_b ? true : false;
  bool chk_best_mva_score = 0.9 < event.best_mva_score ? true : false;
  bool chk_n_b_tagged = 3 <= event.n_bjets ? true : false;
  bool chk_n_c_tagged = event.n_cjets == 1 ? true : false;
  bool chk_c_tagged_w_u = (cvsb_wp_m < event.cvsb_w_u && cvsl_wp_m < event.cvsl_w_u) ? true : false;
  bool chk_b_tagged_w_d = bvsc_wp_m < event.bvsc_w_d ? true : false;

  // Control 0 : 2b
  if (!chk_n_b_tagged)
    return "Control0";

  // Signal region : 3b
  else if (chk_n_b_tagged)
    return "Signal";

  // //Control 1 : c-inversion
  // else if(chk_b_tagged_had_t_b && chk_b_tagged_lep_t_b &&
  //         chk_best_mva_score &&
  //         !chk_n_c_tagged &&
  //         !chk_c_tagged_w_u &&
  //         chk_n_b_tagged &&
  //         chk_b_tagged_w_d) region = "Control1";

  // //Control 2 : score inversion
  // else if(chk_b_tagged_had_t_b && chk_b_tagged_lep_t_b &&
  //         !chk_best_mva_score &&
  //         chk_n_c_tagged &&
  //         chk_c_tagged_w_u &&
  //         chk_n_b_tagged &&
  //         chk_b_tagged_w_d) region = "Control2";

  else
    return "Out";

  return "Out";
} // void Histo_Syst::Set_Region()

//////////
