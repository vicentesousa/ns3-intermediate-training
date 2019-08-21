function vtResult = readMetrics( chFile, parametersName )
% ATENÇÃO: se o chFile for indexado pelo ID do usuário, não esquecer de
% mudar o nome do arquivo. Ao indicar somente o novo id, o arquivo não
% mudará de nome!!!

% % Check specified id inside chFile
% vtloc0 = strfind( chFile, '/' );
% chFileAux = chFile(vtloc0(end)+1:end);
% vtloc = strfind( chFileAux, 'nSTAs' );
% vtloc2 = strfind( chFileAux(vtloc+6:end), '_' );
% readId =  str2num( chFileAux(vtloc+6:vtloc+6+vtloc2(1)-2) );
% if (readId ~= id )
%     error('Erro: selecione arquivo correspondente ao userId');
% end


% Import data from the file
disp(['loading ' chFile])
resultData = importdata( chFile );
% Read Header
chHeader = resultData.textdata;
%chHeader = resultData{1};

% Get parameters
paramCell = [];
for is = 1: length(chHeader)
    paramCell = chHeader{is};
    paramCell(strfind(paramCell,' ')) =[];
    if ( strcmp(paramCell, parametersName) )
        indexParam = is;
        break;
    end
end
% Isolate dat for a specified id
%vtID = resultData.data(:,1);
%idIndex = find(vtID == id);
vtResult = resultData.data(:,indexParam);
