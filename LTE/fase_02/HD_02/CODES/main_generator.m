%% Generate fading traces
clc; clear all; close all;

% Fading Pedestrian EPA 3kmph
f_fading_trace_generator('EPA', 3);
% Fading Vehicular EVA 60kmph
f_fading_trace_generator('EVA', 60)
% Fading Urban ETU 3 kmph
f_fading_trace_generator('ETU', 3);