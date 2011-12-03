function varargout = gui_matlab(varargin)
% GUI_MATLAB MATLAB code for gui_matlab.fig
%      GUI_MATLAB, by itself, creates a new GUI_MATLAB or raises the existing
%      singleton*.
%
%      H = GUI_MATLAB returns the handle to a new GUI_MATLAB or the handle to
%      the existing singleton*.
%
%      GUI_MATLAB('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_MATLAB.M with the given input arguments.
%
%      GUI_MATLAB('Property','Value',...) creates a new GUI_MATLAB or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before gui_matlab_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to gui_matlab_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help gui_matlab

% Last Modified by GUIDE v2.5 03-Dec-2011 14:54:24

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @gui_matlab_OpeningFcn, ...
                   'gui_OutputFcn',  @gui_matlab_OutputFcn, ...
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


% --- Executes just before gui_matlab is made visible.
function gui_matlab_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gui_matlab (see VARARGIN)

% Choose default command line output for gui_matlab
handles.output = hObject;
clc;
handles.macroFolder = cd;
strIniFile = fullfile(handles.macroFolder, 'Magic.mat');
	if exist(strIniFile, 'file')
		% Pull out values and stuff them in structure initialValues.
		initialValues = load('Magic.mat');
		% Assign the image folder from the lastUsedImageFolder field of the
		% structure.
	    handles.ImageFolder = initialValues.lastUsedImageFolder;
	else
		% If the file is not there, point the image folder to the current
		% directory.  Then save it out in our mat file.
		handles.ImageFolder = cd;
		% Save the image folder in our ini file.
		lastUsedImageFolder = handles.ImageFolder;
		save(strIniFile, 'lastUsedImageFolder');
	end
 
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gui_matlab wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = gui_matlab_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in SelectImageFolder.
function SelectImageFolder_Callback(hObject, eventdata, handles)
% hObject    handle to SelectImageFolder (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

 folder = uigetdir(handles.ImageFolder,'Select folder');
	% returnValue will be 0 (a double) if they click cancel.
	% returnValue will be the path (a string) if they clicked OK.
	if folder ~= 0
		% Assign the value if they didn't click cancel.
		handles.ImageFolder = folder;
		ListOfImageNames = {};
        if ~isempty(handles.ImageFolder) 
            if exist(folder,'dir') == false
                msgboxw(['Folder ' folder ' does not exist.']);
                return;
            end
        else
            msgboxw('No folder specified as input for function LoadImageList.');
            return;
        end
    % If it gets to here, the folder is good.
        ImageFiles = dir([handles.ImageFolder '/*.*']);
        for Index = 1:length(ImageFiles)
            baseFileName = ImageFiles(Index).name;
            [folder, name, extension] = fileparts(baseFileName);
            extension = upper(extension);
            switch lower(extension)
                case {'.png', '.bmp', '.jpg', '.tif'}
                % Allow only PNG, TIF, JPG, or BMP images
                ListOfImageNames = [ListOfImageNames baseFileName];
                otherwise
            end
        end
        set(handles.ImageList,'string',ListOfImageNames);
		% Save the image folder in our ini file.
		lastUsedImageFolder = handles.ImageFolder;
		save('Magic.mat', 'lastUsedImageFolder');
	end

% --- Executes on button press in Options.
function Options_Callback(hObject, eventdata, handles)
% hObject    handle to Options (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 optionsGUI();

% --- Executes on button press in IntermediateResults.
function IntermediateResults_Callback(hObject, eventdata, handles)
% hObject    handle to IntermediateResults (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of IntermediateResults


% --- Executes on button press in checkbox20.
function checkbox20_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox20 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox20


% --- Executes on button press in checkbox21.
function checkbox21_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox21 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox21


% --- Executes on button press in checkbox22.
function checkbox22_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox22 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox22


% --- Executes on button press in checkbox23.
function checkbox23_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox23 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox23


% --- Executes on button press in checkbox24.
function checkbox24_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox24 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox24


% --- Executes on button press in checkbox25.
function checkbox25_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox25 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox25


% --- Executes on button press in RGB2IHLS.
function RGB2IHLS_Callback(hObject, eventdata, handles)
% hObject    handle to RGB2IHLS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of RGB2IHLS


% --- Executes on button press in checkbox27.
function checkbox27_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox27 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox27


% --- Executes on button press in checkbox28.
function checkbox28_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox28 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox28


% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in Process.
function Process_Callback(hObject, eventdata, handles)
% hObject    handle to Process (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in SaveAll.
function SaveAll_Callback(hObject, eventdata, handles)
% hObject    handle to SaveAll (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in ImageList.
function ImageList_Callback(hObject, eventdata, handles)
% hObject    handle to ImageList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ImageList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ImageList
 selectedIndex = get(handles.ImageList, 'value');
 ListOfImageNames = get(handles.ImageList, 'string');
% baseImageFileName = ListOfImageNames(selectedIndex);
  baseImageFileName = strcat(cell2mat(ListOfImageNames(selectedIndex)));
 fullImageFileName = [handles.ImageFolder '/' baseImageFileName];
 imageArray = imread(fullImageFileName);

 axes(handles.ImgDisp);
 hold off;	% IMPORTANT NOTE: hold needs to be off in order for the "fit" feature to work correctly.
 axesChildHandle = imshow(imageArray, [], 'InitialMagnification', 'fit');

% --- Executes during object creation, after setting all properties.
function ImageList_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ImageList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in Previous.
function Previous_Callback(hObject, eventdata, handles)
% hObject    handle to Previous (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% --- Executes on button press in Previous.

function Next_Callback(hObject, eventdata, handles)
% hObject    handle to Previous (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --- Executes when figure1 is resized.
function figure1_ResizeFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
