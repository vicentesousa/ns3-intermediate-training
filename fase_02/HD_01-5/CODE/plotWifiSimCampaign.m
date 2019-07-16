function plotWifiSimCampaign( campaignFile )
% Creates campaign folder structure according to parameters variation
% specified in  campaignFile
% campaignFile: file with campaign specification  (complete path)
%
% Usage:
%
% Akemi: plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_RateAlg_Load_CampaignSet.m')
%
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/TCC_rodrigo.m')
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_simTime_CampaignSet.m')
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_RateAlg_Load_CampaignSet.m')
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_OffRate_CampaignSet.m')
%
% TCP
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_simTime_TCP_CampaignSet.m')
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_RateAlg_Load_TCP_CampaignSet.m')
% plotWIFISimCampaign( '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_OffRate_TCP_ampaignSet.m')
%
%
close all;clc;
satisRateCriterion = 1e6;
%
addpath(pwd);
% File name deep level
nameLevel = 2;
% Number of jobs
nRepetitions = 10;
% Create simulation campaign
% Plot 01: average user T-put DL and UL
stPlots.plot01Hand = figure;
hold on;        
% Plot 02: Nav+Modem Energy Consumption DL and UL
%stPlots.plot02Hand = figure;
%hold on;        
% Plot 03: Average Power consumption DL and UL
%stPlots.plot03Hand = figure;
%hold on;        
% Plot 04: PLR DL and UL
stPlots.plot04Hand = figure;
hold on;        
%
stPlots.xLabel = [];
fidsp = fopen(campaignFile, 'r' );
if ( fidsp < 0 )
    disp(['There is not campaign file: ' campaignFile ' is not found!!']);
    return;
end
fclose(fidsp);

% find how many blank caracter before the first valid line and get
% this tabulation
baseTab = findTab(campaignFile);

% %get full path name
% basicFile = which(basicFile);
% [basicFolder,NAME,EXT] = fileparts(basicFile);
campaignFile = which(campaignFile);
[campaignFolder,NAMEcampaign,EXTcampaign] = fileparts(campaignFile);
% Simulation folder
finalFolder = [campaignFolder filesep NAMEcampaign ];
% Create a logical hierachy
sim = createHierarchy( campaignFile, baseTab );

dirLevel = 0;
simIndex = 1;
paramSet = [];
iPoints = 1;
stPlots.vtTputAverageDL = [];
stPlots.vtTputStdDL = [];
stPlots.vtSatisfectionUsrTputDL = [];
stPlots.vtPowerConsumption = [];

stPlots.vtPLRDL = [];
stPlots.vtPLRStdDL = [];
stPlots.vtTputAverageUL = [];
stPlots.vtTputStdUL = [];
stPlots.vtSatisfectionUsrTputUL = [];
stPlots.vtPLRUL = [];
stPlots.vtPLRStdUL = [];

stPlots.chLengendDLAndUL = [];
stPlots.chLegendSingle = [];


% build folder and simulation files structures
for is = 1: length(sim)
    clc;
    disp(['Progress ' num2str(is/length(sim)*100) '% ..., repetitions = ' num2str(nRepetitions) ]);
    % State machine of hierarchy
    if ( sim(is).hierarchy  == 1 & is == 1) % Loop begin
        paramSet = [];
    elseif ( sim(is).hierarchy  == 1 & sim(is).hierarchy==sim(is-1).hierarchy) % same hierarchy at level 1
        % do nothing: only ready the common parameters
    elseif ( sim(is).hierarchy > dirLevel ) % Higher hierarchy
        % do nothing: only read the next parameter
    elseif ( sim(is).hierarchy == dirLevel ) % The same hierarchy
        % file name
        finalName = mountFileName(nameLevel,paramSet);
        % Get results
        %copyAndWriteParamFile( basicFile, finalFolder, finalName, paramSet, vtRunSeed, nRepetitions, NAMEcampaign, simFileName);
        [params, resultFiles]  = readSimParamsAndResultFiles( finalName, finalFolder );
        
        % load DL and UL Results of the current file
        stPlots = readDLAndULResults( stPlots, resultFiles, finalFolder, nRepetitions, satisRateCriterion, paramSet, iPoints );
        
        iPoints = iPoints + 1;
        
        % update simulation Index
        simIndex = simIndex + 1;
        %finalName =
        % update parameSet
        paramSet(end) = [];
    else % Lower hierarchy
        % file name
        finalName = mountFileName(nameLevel,paramSet);
        % Plot a complete line
        [params, resultFiles]  = readSimParamsAndResultFiles( finalName, finalFolder );
        %
        % load DL and UL Results of the current file
        stPlots = readDLAndULResults( stPlots, resultFiles, finalFolder, nRepetitions, satisRateCriterion, paramSet, iPoints );
        
        stPlots = mountLegend(paramSet, stPlots);
        
        plotFigures(stPlots, nRepetitions);
        
        %
        nLevels = (sim(is-1).hierarchy - sim(is).hierarchy);
        % update parameSet
        paramSet(end-nLevels:end) = [];
        % update simulation Index
        simIndex = simIndex + 1;
                
        stPlots.vtTputAverageDL = [];
        stPlots.vtTputStdDL = [];
        stPlots.vtSatisfectionUsrTputDL = [];
        stPlots.vtPowerConsumption = [];
        stPlots.vtPLRDL = [];
        stPlots.vtPLRStdDL = [];
        
        stPlots.vtTputAverageUL = [];
        stPlots.vtTputStdUL = [];
        stPlots.vtSatisfectionUsrTputUL = [];
        stPlots.vtPLRUL = [];
        stPlots.vtPLRStdUL = [];
        
        
        iPoints = 1;
        
    end
    % new parameters to be included in the simulation file
    paramSet = [paramSet; {[sim(is).param ' ' sim(is).value]}];
    % update hierarchy level
    dirLevel = sim(is).hierarchy;
end
% file name
finalName = mountFileName(nameLevel,paramSet);
% Plot a complete line
[params, resultFiles]  = readSimParamsAndResultFiles( finalName, finalFolder );
%
% load DL and UL Results of the current file
stPlots = readDLAndULResults( stPlots, resultFiles, finalFolder, nRepetitions, satisRateCriterion, paramSet, iPoints );

stPlots = mountLegend(paramSet, stPlots);

plotFigures(stPlots, nRepetitions);

% Plot 01: average user T-put DL and UL
figure(stPlots.plot01Hand);
legend(stPlots.chLegendSingle)
%title('Average STA T-put')
xlabel(stPlots.xLabel)
ylabel('Vazão útil [bps]')
ylim([2.0E7 3.0E7])
saveas(gcf, [finalFolder filesep NAMEcampaign '_Tput'], 'fig')
saveas(gcf, [finalFolder filesep NAMEcampaign '_Tput'], 'png')

% Plot 02: Users satisfaction DL and UL
% figure(stPlots.plot02Hand);
% legend(stPlots.chLengendDLAndUL)
% title([ 'User staisfaction -  Criterion = ' num2str(satisRateCriterion/1e3) ' kbps'])
% xlabel(stPlots.xLabel)
% ylabel('User Satisfaction (%)')
% saveas(gcf, [finalFolder filesep NAMEcampaign '_Satisfaction'], 'fig')
% saveas(gcf, [finalFolder filesep NAMEcampaign '_Satisfaction'], 'png')

% Plot 03: Average Power consumption DL and UL
% figure(stPlots.plot03Hand);
% legend(stPlots.chLengendDLAndUL)
% title([ 'AUV Power Consumption'])
% xlabel(stPlots.xLabel)
% ylabel('Power Consumption (J)')
% saveas(gcf, [finalFolder filesep NAMEcampaign '_PowerConsumption'], 'fig')
% saveas(gcf, [finalFolder filesep NAMEcampaign '_PowerConsumption'], 'png')


% Plot 04: PLR DL and UL
figure(stPlots.plot04Hand);
legend(stPlots.chLegendSingle)
title([ 'Packet Loss Ratio'])
xlabel(stPlots.xLabel)
ylabel('PLR')
saveas(gcf, [finalFolder filesep NAMEcampaign '_PLR'], 'fig')
saveas(gcf, [finalFolder filesep NAMEcampaign '_PLR'], 'png')
ylim([0.9 0.95])

end

function copyAndWriteParamFile( basicFile, finalFolder, finalName, paramSet, vtRunSeed, nRepetitions, NAMEcampaign, simFileName)
for jobId = 1:nRepetitions
    ResDirthisJob = [finalFolder filesep 'JOB' num2str(jobId)];
    % Copy new parameter file to destination folder
    sourceFile = basicFile;
    destinationFile =  [ResDirthisJob filesep 'Sim' finalName '.sim'];
    [SUCCESS,MESSAGE,MESSAGEID] = copyfile(sourceFile,destinationFile);
    fidsp = fopen(destinationFile, 'a' );
    if ( fidsp == 0 )
        disp('It could not opend the copy of simParam.m !!!');
        return;
    end
    fwrite( fidsp,sprintf('\n'));
    % Write down new parameters to copied file
    for ir = 1: length(paramSet)
        fwrite( fidsp,paramSet{ir},'char');
        fwrite( fidsp,sprintf('\n'));
    end
    %
    
    % Write down runSeed
    fwrite( fidsp,[ 'global RngRun' ' "' num2str(vtRunSeed(jobId)) '"' ],'char');
    fwrite( fidsp,sprintf('\n'));
    
    
    fclose(fidsp);
    
    % Write down final .sh file
    campaignFile = [NAMEcampaign '.sh'];
    fidcf = fopen(campaignFile, 'a' );
    if ( fidcf < 0 )
        disp(['Problem to open campaign file.  !!!']);
        return;
    end
    
    commandLine = ['./waf  --cwd ' ResDirthisJob ' --run="scratch/' simFileName ...
        ' --ns3::ConfigStore::Filename=' destinationFile ' --ns3::ConfigStore::Mode=Load"'];
    
    %commandLine = ['./waf  --cwd ' ResDirthisJob ' --run="scratch/' simFileName ...
    %    ' --ns3::ConfigStore::Filename=' destinationFile '"'];
    
    
    fwrite( fidcf,commandLine,'char');
    fwrite( fidcf,sprintf('\n'));
    fclose(fidcf);
    %
end
end

function finalName = mountFileName(nameLevel, paramSet)
finalName = [];
for ir = 0:nameLevel-1
    paramStr = paramSet{end+ir-nameLevel+1};
    index = find( paramStr == ' ');
    param = paramStr(1:index(end-1)-1);
    value = paramStr(index(end)+1:end);
    twodots1 = strfind(param, '::');
    twodots2 = strfind(value, '::');
    space1 = find(param == ' ');
    space2 = find(value == ' ');
    if (~isempty(twodots1))
        finalName = [finalName ' ' param(twodots1(end)+2:end)];
    elseif (~isempty(space1))
        finalName = [finalName param(space1(end):end)];
    else
        finalName = [finalName ' ' param];
    end
    if (~isempty(twodots2))
        finalName = [finalName ' ' value(twodots2(end)+2:end)];
    elseif (~isempty(space2))
        finalName = [finalName value(space2(end):end)];
    else
        finalName = [finalName ' ' value];
    end
end
finalName(find(finalName=='"')) =[];
finalName(find(finalName==' ')) ='_';
end

function baseTab = findTab(campaignFile)

fid = fopen(campaignFile, 'r' );
if ( fid < 0 )
    disp(['There is not campaign file: ' campaignFile ' is not found!!']);
    return;
end

goOn = 1;
baseTab = 1;
while goOn
    tline = fgetl(fid);
    % find if line is commented
    isCommented = find(tline == '%');
    if (isempty(isCommented)), isCommented = 0; elseif ( isCommented(1) == 1 ), isCommented = 1; else isCommented = 0; end
    
    if ~ischar(tline), break, end
    
    if ( ~isCommented & ~isempty(tline))
        % number of spaces in the begging of a line
        blankLength = find(diff(find( tline == ' ')) ~= 1 );
        if ( blankLength(1) > 1 )
            baseTab = blankLength(1);
            goOn = 0;
        end
    end
end
fclose(fid);
end


function sim = createHierarchy( campaignFile, baseTab )

fid = fopen(campaignFile, 'r' );
if ( fid < 0 )
    disp(['There is not campaign file.  !!!' campaignFile ' is not found!!']);
    return;
end
blankPosition = 0;
is = 1;

% Get each line of campaign set and create a logical hierachy
while 1
    tline = fgetl(fid);
    % find delimeter
    dPosition = find(tline == '@');
    
    % find if line is commented
    isCommented = find(tline == '%');
    if (isempty(isCommented)), isCommented = 0; elseif ( isCommented(1) == 1 ), isCommented = 1; else isCommented = 0; end
    
    if ~ischar(tline), break, end
    disp(tline)
    
    % test if link is blank or commented
    if ( ~isempty(dPosition) && ~isCommented )
        
        % get blank space
        thisLineBlank = find(tline == ' ');
        
        % number of spaces in the begging of a line
        if ( isempty(thisLineBlank) )
            sim(is).blankLength = 0;
        elseif (thisLineBlank(1) > 1)
            sim(is).blankLength = 0;
        else
            sim(is).blankLength = find(diff(find( tline == ' ')) ~= 1 );
            sim(is).blankLength = sim(is).blankLength(1);
        end
        
        % Classify each parameter in their levels
        if ( sim(is).blankLength == 0 ) % começo da hierarquia
            % hierarchy indicator
            sim(is).hierarchy = 1;
        elseif ( sim(is).blankLength >  blankPosition ) % subindo na hierarquia
            % hierarchy indicator
            sim(is).hierarchy = sim(is-1).hierarchy + 1;
        elseif ( sim(is).blankLength ==  blankPosition )
            % hierarchy indicator
            sim(is).hierarchy = sim(is-1).hierarchy;
        else % descendo na hierarquia
            % hierarchy indicator
            nLevels = (sim(is-1).blankLength - sim(is).blankLength)/baseTab;
            sim(is).hierarchy = sim(is-1).hierarchy - nLevels;
        end
        if ( (sim(is).blankLength)~=0 ) % começo da hierarquia
            % store the initial blank position for next line decision
            blankPosition = thisLineBlank(end);
        else
            blankPosition = 0;
        end
        % find delimeter
        dPosition = find(tline == '@');
        % parameter name
        sim(is).param = tline(1:dPosition-1);
        % remove initial black spaces
        blankLength = find(diff(find( sim(is).param == ' ')) ~= 1 );
        if (~isempty(blankLength))
            sim(is).param(1:blankLength(1)) = [];
        end
        % parameter value
        sim(is).value = tline(dPosition+1:end);
        is = is + 1;
    end
end
fclose(fid);
end


function [params, resultFiles]  = readSimParamsAndResultFiles( finalName, finalFolder )
%vtParams = {'rateAdaptationAlg';'nSTAs';'nAPs';'simTime';'offeredDataRate';'qosEnabled'};
vtParams = {'rateAdaptationAlg';'nSTAs';'nAPs';'simTime';'offeredDataRate'};
vtParams = {'shortGuardIntervalG';'ApStaManagerG'};

dirJob1 = [finalFolder filesep 'JOB1' ];
simFile =  [dirJob1 filesep 'Sim' finalName '.sim'];
fileCell = importdata(simFile);

for ik = 1: length(vtParams)
    paramName = vtParams{ik};
    findParam = strfind(fileCell, paramName);
    paramIndex = find(~cellfun(@isempty,findParam));
    paramLine = fileCell(paramIndex(end));
    paramLine = paramLine{1};
    valueIndex = strfind(paramLine, '"');
    paramValue{ik} = paramLine(valueIndex(1)+1:valueIndex(2)-1);
    if (strcmp(paramValue{ik},'false'))
        paramValue{ik} = '0';
    elseif (strcmp(paramValue{ik},'true'))
        paramValue{ik} = '1';
    end
    %if ( strncmp('ns3::',paramValue{ik},5) )
    %    paramValue{ik} = paramValue{ik}(6:end);
    %end
end
resultsFileUL = ['UL_Results_Sim_shortGuardIntervalG_' paramValue{1} '_ApStaManagerG_' paramValue{2} '.txt'];
resultsFileDL = ['DL_Results_Sim_shortGuardIntervalG_' paramValue{1} '_ApStaManagerG_' paramValue{2} '.txt'];

% resultsFileUL = ['UL_Results_Sim_PropModel_' paramValue{1}...
%     '_nSTAs_' paramValue{2} '_nAPs_' paramValue{3} '_simTime_' paramValue{4}...
%     '_Rate_' num2str(eval(paramValue{5}),'%10.4e\n') '.txt'];
% resultsFileDL = ['DL_Results_Sim_' paramValue{1}...
%     '_nSTAs_' paramValue{2} '_nAPs_' paramValue{3} '_simTime_' paramValue{4}...
%     '_Rate_' num2str(eval(paramValue{5}),'%10.4e\n') '.txt'];


%DL_Results_Sim_PropModel_UanPropModelIdeal_MaxRange_10%
%resultsFileEnergy = ['RateAlg_' paramValue{1}...
%    '_nSTAs_' paramValue{2} '_nAPs_' paramValue{3} '_simTime_' paramValue{4}...
%    '_Rate_' num2str(eval(paramValue{5}),'%10.4e\n') '_Energy.txt'];


% resultsFileUL = ['RateAlg_' paramValue{1} '_QoS_' paramValue{6} ...
%     '_nSTAs_' paramValue{2} '_nAPs_' paramValue{3} '_simTime_' paramValue{4}...
%     '_Rate_' num2str(eval(paramValue{5}),'%10.4e\n') '_UL_Results.txt'];
% resultsFileDL = ['RateAlg_' paramValue{1} '_QoS_' paramValue{6}...
%     '_nSTAs_' paramValue{2} '_nAPs_' paramValue{3} '_simTime_' paramValue{4}...
%     '_Rate_' num2str(eval(paramValue{5}),'%10.4e\n') '_DL_Results.txt'];
% 
% resultsFileEnergy = ['RateAlg_' paramValue{1} '_QoS_' paramValue{6}...
%     '_nSTAs_' paramValue{2} '_nAPs_' paramValue{3} '_simTime_' paramValue{4}...
%     '_Rate_' num2str(eval(paramValue{5}),'%10.4e\n') '_Energy.txt'];


resultFiles.resultsFileDL = resultsFileDL;
resultFiles.resultsFileUL = resultsFileUL;
%resultFiles.resultsFileEnergy = resultsFileEnergy;
params.paramValues = paramValue;
params.paramNames = vtParams;
end

function stPlots = readDLAndULResults( stPlots, resultFiles, finalFolder, nRepetitions, satisRateCriterion, paramSet, iPoints )

vtAllJobsTputDL = [];
vtAllJobsTputUL = [];
vtAllJobsPowerConsumption = [];
vtAllJobsPLRDL = [];
vtAllJobsPLRUL = [];
vtAllJobsNavUL =[];
vtAllJobsNavModemUL = [];
% Read each Job
for jobId = 1:nRepetitions
    % load DL Tput and Packet_Loss_Ratio
    resDirDL = [ finalFolder filesep 'JOB' num2str(jobId) filesep resultFiles.resultsFileDL];
    vtAllJobsTputDL = [ vtAllJobsTputDL readMetrics( resDirDL, 'T_put(Mbps)' ).'*1e6];
    vtAllJobsPLRDL = [vtAllJobsPLRDL readMetrics( resDirDL, 'Packet_Loss_Ratio' ).'];
    % load UL Tput
    resDirUL = [ finalFolder filesep 'JOB' num2str(jobId) filesep resultFiles.resultsFileUL];
    %vtAllJobsTputUL = [ vtAllJobsTputUL readMetrics( resDirUL, 'T_put(Mbps)' ).'*1e6];
    %vtAllJobsPLRUL = [vtAllJobsPLRUL readMetrics( resDirUL, 'Packet_Loss_Ratio' ).'];
    %vtAllJobsNavUL = [vtAllJobsNavUL readMetrics( resDirUL, 'NavComsumption' ).'];
    %vtAllJobsNavModemUL = [vtAllJobsNavModemUL readMetrics( resDirUL, 'NavModemComsumption' ).'];
    % load power consumption
    %resDirEnergy = [ finalFolder filesep 'JOB' num2str(jobId) filesep resultFiles.resultsFileEnergy];
    %vtAllJobsPowerConsumption = [vtAllJobsPowerConsumption readMetrics( resDirEnergy, 'STAEnergyConsumption' ).'];
end

% line of DL results
stPlots.vtTputAverageDL(iPoints) = mean(vtAllJobsTputDL);
stPlots.vtTputStdDL(iPoints) = std(vtAllJobsTputDL);
stPlots.vtSatisfectionUsrTputDL(iPoints) = length(find(vtAllJobsTputDL > satisRateCriterion))/length(vtAllJobsTputDL);
stPlots.vtPLRDL(iPoints) = mean(vtAllJobsPLRDL);
stPlots.vtPLRStdDL(iPoints) = std(vtAllJobsPLRDL);
% line of UL results
% stPlots.vtTputAverageUL(iPoints) = mean(vtAllJobsTputUL);
% stPlots.vtTputStdUL(iPoints) = std(vtAllJobsTputUL);
% stPlots.vtNavmeanUL(iPoints) = mean(vtAllJobsNavUL);
% stPlots.vtNavstdUL(iPoints) = std(vtAllJobsNavUL);
% stPlots.vtNavModemmeanUL(iPoints) = mean(vtAllJobsNavModemUL);
% stPlots.vtNavModemstdUL(iPoints) = std(vtAllJobsNavModemUL);
% stPlots.vtSatisfectionUsrTputUL(iPoints) = length(find(vtAllJobsTputUL > satisRateCriterion))/length(vtAllJobsTputUL);
% stPlots.vtPowerConsumption(iPoints) = mean(vtAllJobsPowerConsumption);
% stPlots.vtPLRUL(iPoints) = mean(vtAllJobsPLRUL);
% stPlots.vtPLRStdUL(iPoints) = std(vtAllJobsPLRUL);
% Update Xaxis (only for numerical values)
paramLine = paramSet{end};
valueIndex = strfind(paramLine, '"');
currentParam = paramLine(valueIndex(1)+1:valueIndex(2)-1);
if ( strncmp('ns3::',currentParam,5) )
    currentParam = currentParam(6:end);
end
stPlots.vtXaxis(iPoints) = str2num(currentParam);


valueIndex2 = strfind(paramLine, ' ');
paramName = paramLine(valueIndex2(1)+1:valueIndex2(2)-1);

% Define xLabel
if ( isempty(stPlots.xLabel) )
    stPlots.xLabel = [paramName];
end

end


function stPlots = mountLegend(paramSet, stPlots)

paramLine = paramSet{end-1};
valueIndex = strfind(paramLine, '"');
paramValue = paramLine(valueIndex(1)+1:valueIndex(2)-1);
if ( strncmp('ns3::',paramValue,5) )
    paramValue = paramValue(6:end);
end
valueIndex2 = strfind(paramLine, ' ');
paramName = paramLine(valueIndex2(1)+1:valueIndex2(2)-1);

chLegendCommon = [paramName ' = ' paramValue ];
stPlots.chLengendDLAndUL = [stPlots.chLengendDLAndUL; {['Nav+Modem - ' chLegendCommon]}];
stPlots.chLengendDLAndUL = [stPlots.chLengendDLAndUL; {['Nav - ' chLegendCommon]}];
stPlots.chLegendSingle = [stPlots.chLegendSingle ; {[chLegendCommon]}];

end

function plotFigures(stPlots, nRepetitions)

% Plot 01: average user T-put DL and UL
figure(stPlots.plot01Hand);
% [vtError_DL] = calculateCI(stPlots.vtTputAverageDL, stPlots.vtTputStdDL, nRepetitions);
% errorbar(stPlots.vtXaxis, stPlots.vtTputAverageDL, vtError_DL,'-s');
% hold all;
[vtError_DL] = calculateCI(stPlots.vtTputAverageDL, stPlots.vtTputStdDL, nRepetitions);
errorbar(stPlots.vtXaxis, stPlots.vtTputAverageDL, vtError_DL,'-x');


% Plot 02: Users satisfaction DL and UL
%figure(stPlots.plot02Hand);
%plot(stPlots.vtXaxis, stPlots.vtNavModemmeanUL,'-s');
%plot(stPlots.vtXaxis, stPlots.vtSatisfectionUsrTputDL,'-s');
%hold all;
%plot(stPlots.vtXaxis, stPlots.vtSatisfectionUsrTputUL,'-x');


% Plot 03: Average Power consumption DL and UL
%figure(stPlots.plot03Hand);
%subplot(2,1,1);
%plot(stPlots.vtXaxis, stPlots.vtNavModemmeanUL,'-s');
%subplot(2,1,2);
%hold all;
%plot(stPlots.vtXaxis, stPlots.vtNavmeanUL,'-s');

% Plot 04: PLR
figure(stPlots.plot04Hand);
% [vtPLRError_DL] = calculateCI(stPlots.vtPLRDL, stPlots.vtPLRStdDL, nRepetitions);
% errorbar(stPlots.vtXaxis, stPlots.vtPLRDL, vtPLRError_DL,'-s');
% hold all;
[vtPLRError_DL] = calculateCI(stPlots.vtPLRDL, stPlots.vtPLRStdDL, nRepetitions);
errorbar(stPlots.vtXaxis, stPlots.vtPLRDL, vtPLRError_DL,'-x');

end
