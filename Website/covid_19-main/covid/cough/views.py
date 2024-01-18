from django.shortcuts import render
import numpy as np
import joblib
model1= joblib.load('saved_models/cough (1).joblib')
from django.core.files.storage import FileSystemStorage
#from google.colab import files
import librosa
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive

# Create your views here.
def prehome(req):
    
    """uploaded = files.upload()

    # Get the file name
    file_name = list(uploaded.keys())[0]
    file_path = '/content/' + file_name
    
    file_path ='/Users/istiakahmedseyam/covid_19/covid/audio/aaa.wav'"""
   
   
    gauth = GoogleAuth()
    drive = GoogleDrive(gauth)
    file_path ='/Users/istiakahmedseyam/covid_19/covid/audio/aaa.wav'
    gfile = drive.CreateFile()
    gfile['title'] = 'My Audio File.wav'
    gfile['mimeType'] = 'audio/wav'
    gfile.SetContentFile(file_path)
    
    user_features = extract_features(file_path)
    user_X = np.array(user_features['mfccs'].mean(axis=1)).reshape(1, -1)
    user_pred = model1.predict(user_X)
    print(f'Predicted Cough Label: {user_pred[0]}')
    return render(req,"start/includes/prehome.html")
def extract_features(audio_path):
    # Load audio file
    signal, sr = librosa.load(audio_path, sr=None)
    # Extract features Mel-frequency cepstral coefficients
    mfccs = librosa.feature.mfcc(y=signal, sr=sr, n_mfcc=40)

    # Return features
    return {'mfccs': mfccs}