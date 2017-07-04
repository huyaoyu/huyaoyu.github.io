function newcomet(varargin)
%COMET  Comet-like trajectory.
%   COMET(Y) displays an animated comet plot of the vector Y.
%   COMET(X,Y) displays an animated comet plot of vector Y vs. X.
%   COMET(X,Y,p) uses a comet of length p*length(Y).  Default is p = 0.10.
%
%   COMET(AX,...) plots into AX instead of GCA.
%
%   Example:
%       t = -pi:pi/200:pi;
%       comet(t,tan(sin(t))-sin(tan(t)))
%
%   See also COMET3.

%   Charles R. Denham, MathWorks, 1989.
%   Revised 2-9-92, LS and DTP; 8-18-92, 11-30-92 CBM.
%   Copyright 1984-2007 The MathWorks, Inc. 
%   $Revision: 5.12.4.4 $  $Date: 2008/01/21 14:59:45 $

%   Yaohu Hu <huyaoyu@sjtu.edu.cn>, Shanghai Jiao Tong University, 2013
%   Revised Jan-17-2013.
%   Modified the color of the tail. Then the plot looks much more like
%   a dynamic trajectory. The function of parameter 'p' has been also
%   changed. User can set p = -1 to access original defualt functionality
%   of comet. Or, specify a number indicating the tail length.
%   The final tail will not be drawed, leaving the final part of the
%   trajectory on the screen.

% Parse possible Axes input
[ax,args,nargs] = axescheck(varargin{:});

error(nargchk(1,3,nargs,'struct'));

% Parse the rest of the inputs
ori_p = 0.1;
if nargs < 2, x = args{1}; y = x; x = 1:length(y); end
if nargs == 2, [x,y] = deal(args{:}); end
if nargs < 3, p = -1; end
if nargs == 3, [x,y,p] = deal(args{:}); end

if nargs == 3 && ( ~isscalar(p) || ~isreal(p) ||  p <= 1 )
    error('MATLAB:comet:InvalidP', ...
          'The input ''p'' must be a real scalar bigger and 1.');
end

ax = newplot(ax);
if ~ishold(ax)
  [minx,maxx] = minmax(x);
  [miny,maxy] = minmax(y);
  axis(ax,[minx maxx miny maxy])
end
co = get(ax,'colororder');

if size(co,1)>=3,
  % Choose first three colors for head, body, and tail
  head = line('parent',ax,'color',co(1,:),'marker','o','erase','xor', ...
              'xdata',x(1),'ydata',y(1));
  body = line('parent',ax,'color',co(2,:),'linestyle','-','erase','none', ...
              'xdata',[],'ydata',[]);
%   tail = line('parent',ax,'color',co(3,:),'linestyle','-','erase','none', ...
%               'xdata',[],'ydata',[]);
  tail = line('parent',ax,'color',[1,1,1],'linestyle','-','erase','none', ...
      'xdata',[],'ydata',[]);
else
  % Choose first three colors for head, body, and tail
  head = line('parent',ax,'color',co(1,:),'marker','o','erase','xor', ...
              'xdata',x(1),'ydata',y(1));
  body = line('parent',ax,'color',co(1,:),'linestyle','--','erase','none', ...
              'xdata',[],'ydata',[]);
%   tail = line('parent',ax,'color',co(1,:),'linestyle','-','erase','none', ...
%               'xdata',[],'ydata',[]);
  tail = line('parent',ax,'color',[1,1,1],'linestyle','-','erase','none', ...
              'xdata',[],'ydata',[]);
end

m = length(x);
if (p == -1)
    k = round(ori_p*m);
else
    k = round(p);
end

% This try/catch block allows the user to close the figure gracefully
% during the comet animation.
try
    % Grow the body
    for i = 2:k+1
        j = i-1:i;
        set(head,'xdata',x(i),'ydata',y(i))
        set(body,'xdata',x(j),'ydata',y(j))
        drawnow
    end

    % Primary loop
    for i = k+2:m
        j = i-1:i;
        set(head,'xdata',x(i),'ydata',y(i))
        set(body,'xdata',x(j),'ydata',y(j))
        set(tail,'xdata',x(j-k),'ydata',y(j-k))
        drawnow
    end

    % Clean up the tail
%     for i = m+1:m+k
%         j = i-1:i;
%         set(tail,'xdata',x(j-k),'ydata',y(j-k))
%         drawnow
%     end
catch E
    if ~strcmp(E.identifier, 'MATLAB:class:InvalidHandle')
        rethrow(E);
    end
end

function [minx,maxx] = minmax(x)
minx = min(x(isfinite(x)));
maxx = max(x(isfinite(x)));
if minx == maxx
  minx = maxx-1;
  maxx = maxx+1;
end
