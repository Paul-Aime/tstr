function x_conv = convTemp(X,H)
M = length(H);
L = length(X);
x_conv = zeros(1,L+M-1);
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
      tmp = tmp + X(k)*H(n-k+1);
    end
    x_conv(n)=tmp;
  end
end