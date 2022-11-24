#ifndef __Result_Event_H__
#define __Result_Event_H__

#include <TString.h>
#include <TTree.h>
#include <TH1D.h>

#include "W_Event.h"

class Data_MC_Comparison;

class Result_Event : public W_Event
{
  friend class Template;
  friend class Histo_Syst;

 public:
  Result_Event(const TString& a_era, const TString& a_channel, const TString& a_swap_mode);
  virtual ~Result_Event();

  void Setup_Tree(TTree* tree, const bool& chk_syst);
  
 protected:  
  float sf_mu_id;
  float sf_mu_id_down;
  float sf_mu_id_up;

  float sf_mu_iso;
  float sf_mu_iso_down;
  float sf_mu_iso_up;
  
  float sf_mu_trig;
  float sf_mu_trig_down;
  float sf_mu_trig_up;
  
  float sf_pujet_veto;
  
  float weight;
  
  float weight_b_tag;
  float weight_b_tag_hf_down;
  float weight_b_tag_hf_up;
  float weight_b_tag_jes_down;
  float weight_b_tag_jes_up;
  float weight_b_tag_lfstats1_down;
  float weight_b_tag_lfstats1_up;
  float weight_b_tag_lfstats2_down;
  float weight_b_tag_lfstats2_up;
  float weight_b_tag_cferr1_down;
  float weight_b_tag_cferr1_up;
  float weight_b_tag_cferr2_down;
  float weight_b_tag_cferr2_up;
  float weight_b_tag_hfstats1_down;
  float weight_b_tag_hfstats1_up;
  float weight_b_tag_hfstats2_down;
  float weight_b_tag_hfstats2_up;
  
  float weight_c_tag;
  float weight_c_tag_extrap_down;
  float weight_c_tag_extrap_up;
  float weight_c_tag_interp_down;
  float weight_c_tag_interp_up;
  float weight_c_tag_lhe_scale_muf_down;
  float weight_c_tag_lhe_scale_muf_up;
  float weight_c_tag_lhe_scale_mur_down;
  float weight_c_tag_lhe_scale_mur_up;
  float weight_c_tag_ps_fsr_fixed_down;
  float weight_c_tag_ps_fsr_fixed_up;
  float weight_c_tag_ps_isr_fixed_down;
  float weight_c_tag_ps_isr_fixed_up;
  float weight_c_tag_pu_down;
  float weight_c_tag_pu_up;
  float weight_c_tag_stat_down;
  float weight_c_tag_stat_up;
  float weight_c_tag_xsec_br_unc_dyjets_b_down;
  float weight_c_tag_xsec_br_unc_dyjets_b_up;
  float weight_c_tag_xsec_br_unc_dyjets_c_down;
  float weight_c_tag_xsec_br_unc_dyjets_c_up;
  float weight_c_tag_xsec_br_unc_wjets_c_down;
  float weight_c_tag_xsec_br_unc_wjets_c_up;
  float weight_c_tag_jer_down;
  float weight_c_tag_jer_up;
  float weight_c_tag_jes_total_down;
  float weight_c_tag_jes_total_up;

  float weight_lumi;
  float weight_mc;

  float weight_pdf_alternative;
  float weight_pdf_error_set[100];
  float weight_pdf_as_down;
  float weight_pdf_as_up;

  float weight_pileup;
  float weight_pileup_down;
  float weight_pileup_up;
 
  float weight_prefire;
  float weight_prefire_down;
  float weight_prefire_up;
  
  float weight_pujet_veto;
  float weight_pujet_veto_down;
  float weight_pujet_veto_up;

  float weight_scale_variation_1;
  float weight_scale_variation_2;
  float weight_scale_variation_3;
  float weight_scale_variation_4;
  float weight_scale_variation_6;
  float weight_scale_variation_8;
  float weight_top_pt;
  int n_vertex;
  float lepton_pt;
  float lepton_eta;
  int n_jets;
  int n_bjets;
  int n_cjets;
  float pt_leading_jet;
  float pt_subleading_jet;
  float eta_leading_jet;
  float eta_subleading_jet;
  float met_pt;
  float met_phi;
  float best_mva_score;
  float best_chi2;
  float mt;
  float mva_hf_score;
  float bvsc_had_t_b;
  float cvsb_had_t_b;
  float cvsl_had_t_b;
  float bvsc_lep_t_b;
  float cvsb_lep_t_b;
  float cvsl_lep_t_b;
  float pt_had_t_b;
  float pt_lep_t_b;
  
  //For MC
  int decay_mode;
  bool chk_reco_correct;
  bool chk_included;
  bool chk_hf_contamination;
  bool pu_conta_had_t_b;
  bool pu_conta_lep_t_b;
  
  ClassDef(Result_Event, 1);
};

#endif /* __Result_Event_H__  */
