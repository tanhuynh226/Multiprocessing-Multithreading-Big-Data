#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 20 01:30:55 2018

@author: sina

Updated on Thu Apr 7 2022
kennet11
"""
import subprocess
import os
import sys
import pandas as pd
from pathlib import Path

# run make
make_process = subprocess.Popen("make clean && make", shell=True, stdout=subprocess.PIPE, stderr=sys.stdout.fileno())
while True:
    line = make_process.stdout.readline()
    if not line:
        break
    
print('Your program was successfully compiled')
print('Running your program now...')

# run user program
exec_process = subprocess.Popen("./p1_exec 4 6",shell=True, stdout=subprocess.PIPE, stderr=sys.stdout.fileno())
while True:
    line = exec_process.stdout.readline()
    if not line:
        break
    print(line)

print('Checking your outputs')

input_path = Path('input/')
ground_truth_path = Path('grader_outputs/')
user_path = Path('output/')

input_files = [file for file in input_path.iterdir() if file.suffix == '.csv']
for input_csv in input_files:
    test_name = input_csv.stem
    correct_outputs_stats = pd.read_csv(ground_truth_path.joinpath(f'{test_name}_stats.csv').as_posix()).round(3)
    outputs_stats = pd.read_csv(user_path.joinpath(f'{test_name}_stats.csv').as_posix()).round(3)

    
    print(f'Stats for {test_name} test match? {str(correct_outputs_stats.equals(outputs_stats))}\n')
    
    correct_outputs_sorted = pd.read_csv(ground_truth_path.joinpath(f'{test_name}_sorted.csv'))[['Rank', 'Student ID']]
    outputs_sorted = pd.read_csv(user_path.joinpath(f'{test_name}_sorted.csv'))[['Rank', 'Student ID']]
    differences = correct_outputs_sorted.count().sum() - (correct_outputs_sorted == outputs_sorted).astype(int).sum().sum()
    
    print(f'Differences in rows for {test_name} test? {differences}\n');
