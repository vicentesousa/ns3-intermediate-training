function [vtError] = calculateCI(vtMean, vtStd, nRepetitions)

if ( nRepetitions > 30)
    vtSig = vtStd/sqrt(nRepetitions); % sample standard deviation of the mean
    alphao2 = 0.05/2;      % Confidence interval of 95%
    vtConfInter = [vtMean + abs(norminv(alphao2))*vtSig ;...
        vtMean - abs(norminv(alphao2))*vtSig  ];
    vtError = abs(norminv(alphao2))*vtSig;
else
    vtSig = vtStd/sqrt(nRepetitions); % sample standard deviation of the mean
    alphao2 = 0.05/2;      % Confidence interval of 95%
    level = nRepetitions-1;
    vtConfInter = [vtMean - tinv(alphao2,level)*vtSig ;...
        vtMean + tinv(alphao2,level)*vtSig  ];
    vtError = -tinv(alphao2,level)*vtSig;
    
end