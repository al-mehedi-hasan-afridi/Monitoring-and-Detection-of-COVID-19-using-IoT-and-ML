import imp
from django.shortcuts import render

from django.shortcuts import redirect

from django.contrib import messages

from django.contrib.auth import authenticate, login, logout

import numpy as np
#import joblib
#model= joblib.load('saved_models/final_model.joblib')
import pickle
model = pickle.load(open('saved_models/fff_model.pkl', 'rb'))







from django.core.mail import EmailMessage
import pandas

import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

#from django.utils.functional import safestring

cred = credentials.Certificate("cse499-cd2ac-firebase-adminsdk-6gvyp-6109eb67f8.json")
firebase_admin.initialize_app(cred,{'databaseURL': 'https://cse499-cd2ac-default-rtdb.firebaseio.com'})

# Create your views here.


def home(req):
    
    
    data = db.reference('Data')

    
    root = db.reference()
    
    dt = root.child('Data').get()
    print(dt)
    bpm=dt.get("BPM")
    cough=dt.get("Cough")
    spo2=dt.get("SP02")
    temp=dt.get("Temperature")

    """if(temp>36.1):
        fever=1
    else:
        fever=0"""
    flag=cough.split()
    if(flag[0]=="healthy"):
        cough_flag=0
    else:
        cough_flag=1
    
    #input_data=(temp,cough_flag,bpm,spo2)
    #input_data=(1,1,100,96)
    input_data=(36.2,0,108,99)
    input_data_as_numpy_array=np.asarray(input_data)
    reshape=input_data_as_numpy_array.reshape(1,-1)
    prediction=model.predict(reshape)
    print("codid-19 res:")
    print(type(prediction))
    print(prediction[0])
    cv19=prediction[0]
    print(cv19)

    if(prediction==0):
        res='<b>Covid-19 negative(-) suspect</b>'
    else:
        res='<b>Covid-19 positive(+) suspect</b>'

        mail_subject = 'Suspected Covid-19'
        message="Dear, Please be concerned, you are a suspected covid-19 patient. For confirmation please checkup from a nearby Hospital"
        email = EmailMessage(mail_subject, message, to=['aalmehedihasan@gmail.com','chowdhurymaruf36@gmail.com'])
        email.send()

    print(res)

    if req.method=="POST":
        return redirect('record')
    #r = safestring.mark_safe(res)    
    return render(req,"start/includes/home.html",{
        "bpm":bpm,
        "spo2":spo2,
        "temp":temp,
        "cough":cough,
        "covid_res":int(cv19)
    })

def record(req):
    if req.method=="POST":
        return redirect('home')

    return render(req,"start/includes/record.html")
