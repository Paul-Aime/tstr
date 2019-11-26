function x_conv = myconv(x, h)
%MYCONV Compute the convolution between signal x and impulse response h.
%
% INPUTS
%   x - size (L, 1)
%   h - size (M, 1)
%
% RETURNS
%   x_conv - size (L+M-1, 1)

L = size(x, 1);
M = size(h, 1);

x_conv = zeros(L+M-1, 1);

for n=1:L+M-1
    
    tmp = 0;
    
    if (n>=M)
        kmin = n-M+1;
    else 
        kmin = 1;
    end

    if (n<L)
        kmax = n;
    else
        kmax = L;
    end
    
    %fprintf('kmin=%i,kmax=%i,n=%i\n',kmin,kmax,n);
    
    for k=kmin:kmax
        tmp = tmp + x(k) * h(n-k+1);
    end

    x_conv(n)=tmp;
end


end