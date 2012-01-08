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

% Last Modified by GUIDE v2.5 08-Jan-2012 04:59:49

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


%% --- Executes just before gui_matlab is made visible.
function gui_matlab_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gui_matlab (see VARARGIN)

% Choose default command line output for gui_matlab
handles.output = hObject;
clc;

global Options;

% Load all options to the checkboxes
intermediate = false;
if Options.NHS_output
    set(handles.NHSSegmentation, 'Value', 1);
    intermediate = true;
end
if Options.POST_out_noise
    set(handles.NoiseRemoval, 'Value', 1);
    intermediate = true;
end
if Options.POST_out_clean
    set(handles.ObjectElimination, 'Value', 1);
    intermediate = true;
end
if Options.CE_output
    set(handles.ContourExtraction, 'Value', 1);
    intermediate = true;
end
if Options.RO_output
    set(handles.GetRotationalOffset, 'Value', 1);
    intermediate = true;
end
if Options.GIELIS_output
    set(handles.GelisShapeReconstruction, 'Value', 1);
    intermediate = true;
end
if intermediate
    set(handles.IntermediateResults, 'Value', 1);
end

% Remember the previously selected folder 
handles.ImageFolder = Options.lastFolder;
handles = LoadImageList(handles);
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gui_matlab wait for user response (see UIRESUME)
% uiwait(handles.figure1);


%% --- Outputs from this function are returned to the command line.
function varargout = gui_matlab_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


%% --- Executes on button press in SelectImageFolder.
function SelectImageFolder_Callback(hObject, eventdata, handles)
% hObject    handle to SelectImageFolder (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global Options;
folder = uigetdir(handles.ImageFolder,'Select folder');
% returnValue will be 0 (a double) if they click cancel.
% returnValue will be the path (a string) if they clicked OK.
if folder ~= 0
    % Assign the value if they didn't click cancel.
    handles.ImageFolder = folder;
    handles = LoadImageList(handles);
    guidata(hObject, handles);
    % Save the image folder in our Options file.
    Options.lastFolder = handles.ImageFolder;		
    save opt.mat -struct Options;
else
    msgbox('No folder specified as input for function LoadImageList.');
    return;
end
%===========================================================================    
 %% ---- Load up the listbox with image files from the selected folder -----
function handles=LoadImageList(handles)
    ListOfImageNames = {};
    folder = handles.ImageFolder;
    if ~isempty(handles.ImageFolder) 
        if exist(folder,'dir') == false
            msgbox(['Folder ' folder ' does not exist.']);
            return;
        end
    else
        msgbox('No folder specified as input for function LoadImageList.');
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
return;

%========================================================================================
%% --- Executes on button press in Options.
function Options_Callback(hObject, eventdata, handles)
% hObject    handle to Options (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
optionsGUI(); % all options will be loaded from the global variable Options

% --- Executes on button press in IntermediateResults.
function IntermediateResults_Callback(hObject, eventdata, handles)
% hObject    handle to IntermediateResults (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of IntermediateResults


% --- Executes on button press in GetRotationalOffset.
function GetRotationalOffset_Callback(hObject, eventdata, handles)
% hObject    handle to GetRotationalOffset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of GetRotationalOffset


% --- Executes on button press in ContourExtraction.
function ContourExtraction_Callback(hObject, eventdata, handles)
% hObject    handle to ContourExtraction (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of ContourExtraction


% --- Executes on button press in FinalOutput.
function FinalOutput_Callback(hObject, eventdata, handles)
% hObject    handle to FinalOutput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of FinalOutput


% --- Executes on button press in GelisShapeReconstruction.
function GelisShapeReconstruction_Callback(hObject, eventdata, handles)
% hObject    handle to GelisShapeReconstruction (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of GelisShapeReconstruction


% --- Executes on button press in RecoverDeformedShape.
function RecoverDeformedShape_Callback(hObject, eventdata, handles)
% hObject    handle to RecoverDeformedShape (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of RecoverDeformedShape


% --- Executes on button press in NHSSegmentation.
function NHSSegmentation_Callback(hObject, eventdata, handles)
% hObject    handle to NHSSegmentation (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of NHSSegmentation


% --- Executes on button press in RGB2IHLS.
function RGB2IHLS_Callback(hObject, eventdata, handles)
% hObject    handle to RGB2IHLS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of RGB2IHLS


% --- Executes on button press in ObjectElimination.
function ObjectElimination_Callback(hObject, eventdata, handles)
% hObject    handle to ObjectElimination (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of ObjectElimination


% --- Executes on button press in NoiseRemoval.
function NoiseRemoval_Callback(hObject, eventdata, handles)
% hObject    handle to NoiseRemoval (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of NoiseRemoval


% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


%% --- Executes on button press in Process.
function Process_Callback(hObject, eventdata, handles)
% hObject    handle to Process (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Read options
global Options;

im = get(handles.ImageList,'UserData');
if size(im) == 0
   msgbox('No Image Selected from the list');
   return;   
end

% Process the current image
Options.currfilename = get(handles.text1,'string');
[nhs,noiseRem,cleanImg,valid_contour,countourImg,Offset,Output,OutputImg] = ProcessImage(im); % less parameters

% Save all images in a variable
global Images;

fname = Options.currfilename;
fname = fname(1:length(fname)-4);
Images = cell(3, 6); % first row images, second names, and save names

Images{1,1} = im; % input image
Images{2,1} = Options.currfilename; % original name for the input image
Images{3,1} = [fname,'_original.png'];

Images{1,2} = nhs; % after segmentation
Images{2,2} = 'NHS segmentation';
Images{3,2} = [fname,'_nhs.png'];

Images{1,3} = noiseRem; % after noise removal
Images{2,3} = 'Noise removal';
Images{3,3} = [fname,'_noiseRem.png'];

Images{1,4} = cleanImg; % after morphological operations
Images{2,4} = 'Cleaned image';
Images{3,4} = [fname,'_clean.png'];

Images{1,5} = countourImg; % after contour extraction
Images{2,5} = 'Contour extraction';
Images{3,5} = [fname,'_CE.png'];

Images{1,6} = OutputImg; % final output image
Images{2,6} = 'Output image';
Images{3,6} = [fname,'_output.png'];

% Save all files in a variable
global Files;
Files = cell(2,3);

Files{1,1} = valid_contour; % contour extraction
Files{2,1} = [fname,'_CE']; % add numbers when saving

Files{1,2} = Offset; % rotational offset
Files{2,2} = [fname,'_rotOff'];

Files{1,3} = Output; % gielis curves recontruction
Files{2,3} = [fname,'_gielis'];

% Image is processed now
Options.Processflag = 1;
msgbox('Done Processing');


%% --- Executes on button press in SaveSelected.
function SaveSelected_Callback(hObject, eventdata, handles)
% hObject    handle to SaveSelected (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global Options;
global Images;
global Files;
  
if Options.Processflag == 1 % ensure image already processed
     foldername = Options.lastFolder;
     foldername = [foldername '/' 'output'];
     if ~isdir(foldername )
           mkdir(foldername);
     end
     foldername = [foldername '/'];
     
    % Load actual options
    Options.intermediate_output = get(handles.IntermediateResults,'Value');
    Options.NHS_output = get(handles.NHSSegmentation,'Value');
    Options.POST_out_noise = get(handles.NoiseRemoval,'Value');
    Options.POST_out_clean = get(handles.ObjectElimination,'Value');
    Options.CE_output = get(handles.ContourExtraction,'Value');
    Options.RO_output = get(handles.GetRotationalOffset,'Value'); 
    Options.GIELIS_output = get(handles.GelisShapeReconstruction,'Value');
    save opt.mat -struct Options;
    
    if(Options.NHS_output)
        filepath= [foldername Images{3,2}];
        imwrite(Images{1,2},filepath,'PNG','BitDepth',1);
    end
     if(Options.POST_out_noise)
        filepath= [foldername Images{3,3}];
        imwrite(Images{1,3},filepath,'PNG','BitDepth',1);
     end
    if(Options.POST_out_clean)
        filepath= [foldername Images{3,4}];
        imwrite(Images{1,4},filepath,'PNG','BitDepth',1);
    end
     if(Options.CE_output)
        filepath= [foldername Images{3,5}];
        imwrite(Images{1,5},filepath,'PNG','BitDepth',1);
        saveText(1,foldername);
     end
     if(Options.POST_out_noise)
         saveText(2,foldername);   
     end
     if(Options.GIELIS_output)
        filepath= [foldername Images{3,6}];
        imwrite(Images{1,6},filepath,'PNG','BitDepth',8);
        saveText(3,foldername);
     end
     
%     % Images
%     for i = 1:6
%          filepath= [foldername Images{3,i}];
%         if i==1 || i==6 % save color images
%             imwrite(Images{1,i},filepath,'PNG','BitDepth',8);
%         else % save binary images
%             imwrite(Images{1,i},filepath,'PNG','BitDepth',1);
%         end
%     end
    
    % Files
%     for i=1:3
%         datas = Files{1,i};
%         fname = Files{2,i};
%         for j=1:length(datas)
%             data = datas{j};
%             sname = [foldername fname, '_', num2str(j), '.txt']; % save name
%             save(sname,'data','-ASCII');
%         end
%     end
end
%% saves text file
function saveText(i,foldername)
global Files;
datas = Files{1,i};
fname = Files{2,i};
for j=1:length(datas)
    data = datas{j};
    sname = [foldername fname, '_', num2str(j), '.txt']; % save name
    save(sname,'data','-ASCII');
end
return;

%% --- Executes on selection change in ImageList.
function ImageList_Callback(hObject, eventdata, handles)
% hObject    handle to ImageList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ImageList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ImageList
global Options;
selectedIndex = get(handles.ImageList, 'value');
ListOfImageNames = get(handles.ImageList, 'string');

%if no files are loaded in list the list contains [Image List]
%the first char [ is checked for whether the list is empty or not 
flagempty = char(ListOfImageNames(selectedIndex)); 
if  flagempty ~= '['
    baseImageFileName = strcat(cell2mat(ListOfImageNames(selectedIndex)));
    fullImageFileName = [handles.ImageFolder '/' baseImageFileName];
    imageArray = imread(fullImageFileName);
    ImageDisplay(imageArray);
    set(handles.text1,'string',baseImageFileName);
    set(hObject,'UserData',imageArray);
      
    % Reset display index and process flag
    Options.DisplayIndex = 1;
    Options.Processflag = 0;
end


%==========================================================================
%% --- Displays image in image list -------  
function ImageDisplay(imageArray)
hold off;	% IMPORTANT NOTE: hold needs to be off in order for the "fit" feature to work correctly.
axesChildHandle = imshow(imageArray, [], 'InitialMagnification', 'fit');
return;
 
 %==========================================================================
%% --- Plot Image----------
function PlotFigure(ObjectArray)
hold off;
axesChildHandle = plot(ObjectArray(:,2),ObjectArray(:,1),'g','LineWidth', 3);
return;
 
%% --- Executes during object creation, after setting all properties.
function ImageList_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ImageList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


%% --- Executes on button press in Previous.
function Previous_Callback(hObject, eventdata, handles)
% hObject    handle to Previous (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% --- Executes on button press in Previous.
    global Options;
    if Options.Processflag == 1
        updateDisplayIndex('prev',handles);
        ImageShow(handles);
    end
    
%% --- Executes on button press in Next.
function Next_Callback(hObject, eventdata, handles)
% hObject    handle to Previous (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    global Options;
    if Options.Processflag == 1
        updateDisplayIndex('next',handles);
        ImageShow(handles);
    end
return
 
%==========================================================================
%%--- show the intermediate results and o/p--------
function ImageShow(handles)
    global Options;
    global Images;
    if Options.Processflag == 1
        ImageDisplay(Images{1, Options.DisplayIndex});
        set(handles.text1,'string',Images{2, Options.DisplayIndex});
    end
return
%==========================================================================
%% -- update DisplayIndex according to check box
function updateDisplayIndex(buttontype,handles)
    % Function only called if Options.Processflag == 1
    global Options
    
    % Which button is pressed?
    if strcmp(buttontype, 'prev')
        value = -1;
    end
    if strcmp(buttontype, 'next')
        value = 1;
    end
    
    % updates display index according to check boxes
    % selection after each time prev/next button is pressed
    
    % Load actual options
    Options.intermediate_output = get(handles.IntermediateResults,'Value');
    Options.NHS_output = get(handles.NHSSegmentation,'Value');
    Options.POST_out_noise = get(handles.NoiseRemoval,'Value');
    Options.POST_out_clean = get(handles.ObjectElimination,'Value');
    Options.CE_output = get(handles.ContourExtraction,'Value');
    Options.GIELIS_output = get(handles.GelisShapeReconstruction,'Value');
    save opt.mat -struct Options;
    
    % Logic for image to display
    presentdisplay = Options.DisplayIndex + value;
    if Options.intermediate_output
        flag = true; % to know if we are still finding the correct output to display
        while flag
            switch presentdisplay
                case 2
                    if Options.NHS_output
                        flag = 0;
                    else
                        presentdisplay = presentdisplay + value;
                    end
                case 3
                    if Options.POST_out_noise
                        flag = 0;
                    else
                        presentdisplay = presentdisplay + value;
                    end
                case 4
                    if Options.POST_out_clean
                        flag = 0;
                    else
                        presentdisplay = presentdisplay + value;
                    end
                case 5
                    if Options.CE_output
                        flag = 0;
                    else
                        presentdisplay = presentdisplay + value;
                    end
                case 6
                    if Options.GIELIS_output
                        flag = 0;
                    else
                        presentdisplay = presentdisplay + value;
                    end
                otherwise
                     if(Options.DisplayIndex<6 && presentdisplay >1)
                        presentdisplay = 6;
                     end
                    
                    if (presentdisplay > 6) 
                        presentdisplay = 1;
                    elseif(presentdisplay <1) 
                           presentdisplay = 6; 
                    end
                    
                   
                    flag = 0;
            end
             
        end
       
        Options.DisplayIndex = presentdisplay;
    else   % if intermdiate display is not checked ..it just display output
        if presentdisplay ~=6
            presentdisplay =6;
        else
            presentdisplay = 1;
        end
        Options.DisplayIndex = presentdisplay;
    end
        
 return;
                
%============================================================================= 

%% --- Executes when figure1 is resized.
function figure1_ResizeFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
