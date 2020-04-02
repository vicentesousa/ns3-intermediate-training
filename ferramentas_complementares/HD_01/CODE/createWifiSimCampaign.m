function createWifiSimCampaign( basicFile, campaignFile, simFileName )
% Creates campaign folder structure according to parameters variation
% specified in  campaignFile
% basicFile: file with base parameters (complete path)
% campaignFile: file with campaign specification  (complete path)
% simFileName: file simulator binary (only the name)
%
% Usage: 
%
% Akemi: createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFI.sim', '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_RateAlg_Load_CampaignSet.m', 'wifi-network_802_11_Single_Service')
%
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFIMAC.sim', '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/TCC_Rodrigo.m', 'wifi-network_802_11_Single_Service')
%
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFIMAC.sim', '/Users/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_simTime_CampaignSet_01.m', 'wifi-network_802_11_Single_Service')
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFIMAC.sim', '/Users/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_OffRate_CampaignSet_01.m', 'wifi-network_802_11_Single_Service')
%
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFIMAC.sim', '/Users/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_simTime_CampaignSet_02.m', 'wifi-network_802_11_Single_Service')
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFIMAC.sim', '/Users/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_OffRate_CampaignSet_02.m', 'wifi-network_802_11_Single_Service')
%
%
% TCP
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFI.sim', '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_simTime_TCP_CampaignSet.m', 'wifi-network_802_11_Single_Service')
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFI.sim', '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_Load_OffRate_TCP_CampaignSet.m', 'wifi-network_802_11_Single_Service')
% createWIFISimCampaign('/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/paramBaseWIFI.sim', '/home/gppcom/ns3/ns-allinone-3.23/ns-3.23/WIFI_RateAlg_Load_TCP_CampaignSet.m', 'wifi-network_802_11_Single_Service')
%
addpath(pwd);
% File name deep level
nameLevel = 2;
% Number of jobs
nRepetitions = 10;
% Random seeds for each job
vtRunSeed = randi(2.3e15,1,nRepetitions);
% Create simulation campaign

fidsp = fopen(basicFile, 'r' );
if ( fidsp < 0 )
    disp(['There is not inputParameters.m: ' basicFile ' is not found!!']);
    return;
end
fclose(fidsp);

% find how many blank caracter before the first valid line and get
% this tabulation
baseTab = findTab(campaignFile);

%get full path name
basicFile = which(basicFile);
[basicFolder,NAME,EXT] = fileparts(basicFile);
%campaignFile = which(campaignFile);
[campaignFolder,NAMEcampaign,EXTcampaign] = fileparts(campaignFile);

% Delete previous .sh file with the same name
delete([campaignFolder filesep NAMEcampaign '.sh']);

% Create a logical hierachy
sim = createHierarchy( campaignFile, baseTab );

% create simulation campaign folder
finalFolder = [campaignFolder filesep NAMEcampaign ];
[SUCCESS,MESSAGE,MESSAGEID] = mkdir(finalFolder);
if (~SUCCESS)
    disp(MESSAGE);
end
% create a folder for each Job
for jobId = 1:nRepetitions
    %
    % Create result directory
    ResDirthisJob = [finalFolder filesep 'JOB' num2str(jobId)];
    test = dir( ResDirthisJob );
    if isempty( test )
        mkdir( ResDirthisJob );
    end
end
%
dirLevel = 0;
simIndex = 1;
paramSet = [];
% build folder and simulation files structures
for is = 1: length(sim)
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
        % Copy new parameter file to destination folder
        copyAndWriteParamFile( basicFile, finalFolder, finalName, paramSet, vtRunSeed, nRepetitions, NAMEcampaign, simFileName, campaignFolder);
        % update simulation Index
        simIndex = simIndex + 1;
        %finalName =
        % update parameSet
        paramSet(end) = [];
    else % Lower hierarchy
        % file name
        finalName = mountFileName(nameLevel,paramSet);
        % Create new setParameter.m file on destination folder
        copyAndWriteParamFile( basicFile, finalFolder, finalName, paramSet, vtRunSeed, nRepetitions, NAMEcampaign, simFileName, campaignFolder);
        %
        nLevels = (sim(is-1).hierarchy - sim(is).hierarchy);
        % update parameSet
        paramSet(end-nLevels:end) = [];
        % update simulation Index
        simIndex = simIndex + 1;
        %finalName =
    end
    % new parameters to be included in the simulation file
    paramSet = [paramSet; {[sim(is).param ' ' sim(is).value]}];
    % update hierarchy level
    dirLevel = sim(is).hierarchy;
end
%fclose(fidsp);
% file name
finalName = mountFileName(nameLevel,paramSet);
% Create new setParameter.m file on destination folder
copyAndWriteParamFile( basicFile, finalFolder, finalName, paramSet, vtRunSeed, nRepetitions, NAMEcampaign, simFileName, campaignFolder);
end

function copyAndWriteParamFile( basicFile, finalFolder, finalName, paramSet, vtRunSeed, nRepetitions, NAMEcampaign, simFileName, campaignFolder)
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
    campaignFileFinal = [campaignFolder filesep NAMEcampaign '.sh'];
    fidcf = fopen(campaignFileFinal, 'a' );
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
