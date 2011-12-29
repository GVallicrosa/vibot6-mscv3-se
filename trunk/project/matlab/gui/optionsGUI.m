%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         GUI                          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                      %
%    Suman Raj Bista(Manager)          %
%    Pablo Speciale	                   %
%    Nolang Fanani	                   %
%    Nathanael Lemessa Baisa	       %
%    Taman Upadhaya	                   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function varargout = optionsGUI(varargin)
% OPTIONSGUI MATLAB code for optionsGUI.fig
%      OPTIONSGUI, by itself, creates a new OPTIONSGUI or raises the existing
%      singleton*.
%
%      H = OPTIONSGUI returns the handle to a new OPTIONSGUI or the handle to
%      the existing singleton*.
%
%      OPTIONSGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in OPTIONSGUI.M with the given input arguments.
%
%      OPTIONSGUI('Property','Value',...) creates a new OPTIONSGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before optionsGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to optionsGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help optionsGUI

% Last Modified by GUIDE v2.5 29-Dec-2011 12:59:17

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @optionsGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @optionsGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before optionsGUI is made visible.
function optionsGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to optionsGUI (see VARARGIN)

% Choose default command line output for optionsGUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes optionsGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = optionsGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function lowratio_Callback(hObject, eventdata, handles)
% hObject    handle to lowratio (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of lowratio as text
%        str2double(get(hObject,'String')) returns contents of lowratio as a double

% Save changes to Options
global Options;
Options.POST_lowRatio = str2double(get(hObject,'String'));
save opt.mat -struct Options;

% --- Executes during object creation, after setting all properties.
function lowratio_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lowratio (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% Load values from Options
global Options;
set(hObject, 'String', num2str(Options.POST_lowRatio));


function highratio_Callback(hObject, eventdata, handles)
% hObject    handle to highratio (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of highratio as text
%        str2double(get(hObject,'String')) returns contents of highratio as a double

% Save changes to Options
global Options;
Options.POST_highRatio = str2double(get(hObject,'String'));
save opt.mat -struct Options;

% --- Executes during object creation, after setting all properties.
function highratio_CreateFcn(hObject, eventdata, handles)
% hObject    handle to highratio (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% Load values from Options
global Options;
set(hObject, 'String', num2str(Options.POST_highRatio));


function aspectArea_Callback(hObject, eventdata, handles)
% hObject    handle to aspectArea (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of aspectArea as text
%        str2double(get(hObject,'String')) returns contents of aspectArea as a double

% Save changes to Options
global Options;
Options.POST_aspectArea = str2double(get(hObject,'String'));
save opt.mat -struct Options;

% --- Executes during object creation, after setting all properties.
function aspectArea_CreateFcn(hObject, eventdata, handles)
% hObject    handle to aspectArea (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% Load values from Options
global Options;
set(hObject, 'String', num2str(Options.POST_aspectArea));



function distThres_Callback(hObject, eventdata, handles)
% hObject    handle to distThres (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of distThres as text
%        str2double(get(hObject,'String')) returns contents of distThres as
%        a double

% Save changes to Options
global Options;
Options.CE_distError = str2double(get(hObject,'String'));
save opt.mat -struct Options;

% --- Executes during object creation, after setting all properties.
function distThres_CreateFcn(hObject, eventdata, handles)
% hObject    handle to distThres (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% Load values from Options
global Options;
set(hObject, 'String', num2str(Options.CE_distError));


% --- Executes on button press in default.
function default_Callback(hObject, eventdata, handles)
% hObject    handle to default (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global Options;
% NHS
Options.NHS_color  = 'red';      % color to segment 'blue' or 'red'
% PostProcessing
Options.POST_aspectArea = 1500;  % imageArea/aspectArea thresholding (default 1500)
Options.POST_lowRatio   = 0.25;  % height/width lowest value (default 0.25)
Options.POST_highRatio  = 1.3;   % height/width highest value (default 1.3)
% Contour extraction
Options.CE_distError = sqrt(2);  % maximum distance from the convex hull to take contour pixels (default sqrt(2))
% Gielis recontruction
Options.GIELIS_norm = true;      % use normalization (default true)
Options.GIELIS_func = 1;         % cost function to use 1, 2 or 3 (default 1)d
% Create the options file
save opt.mat -struct Options;


% --- Executes during object creation, after setting all properties.
function Red_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Red (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Load values from Options
global Options;
if strcmp(Options.NHS_color, 'red')
    set(hObject, 'Value', [1.0]);
% else
%     set(hObject, 'Value', [0.0]);
end


% --- Executes on button press in Red.
function Red_Callback(hObject, eventdata, handles)
% hObject    handle to Red (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Red
global Options;
if get(hObject,'Value') == 1
    Options.NHS_color = 'red';
    save opt.mat -struct Options;
end


% --- Executes during object creation, after setting all properties.
function Blue_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Blue (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Load values from Options
global Options;
if strcmp(Options.NHS_color, 'blue')
    set(hObject, 'Value', 1);
% else
%     set(hObject, 'Value', 0);
end
global bluebutton;
bluebutton = hObject;


% --- Executes on button press in Blue.
function Blue_Callback(hObject, eventdata, handles)
% hObject    handle to Blue (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Blue
global Options;
if get(hObject,'Value') == 1
    Options.NHS_color = 'blue';
    save opt.mat -struct Options;
end