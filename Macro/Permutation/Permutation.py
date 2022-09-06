#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='Era', default="2018")
parser.add_argument('-ch', dest='Channel', default="Mu")
parser.add_argument('-njet', dest='NJet', default=4)
parser.add_argument('-pre_cut', dest='Pre_Cut', default=False)
parser.add_argument('-final_kin', dest='Final_Kin', default=False)
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import os
path=os.environ['Vcb_Post_Analysis_WD']

import ROOT
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

print(type(int(args.Pre_Cut)))
print(int(args.Pre_Cut))
print(bool(int(args.Pre_Cut)))
print(args.Final_Kin)
permutation = ROOT.Permutation_TMVA(args.Era, args.Channel, int(args.NJet), bool(int(args.Pre_Cut)), bool(int(args.Final_Kin)))
