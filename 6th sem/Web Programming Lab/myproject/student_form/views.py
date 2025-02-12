
# myapp/views.py
from django.shortcuts import render
from .forms import StudentForm

def home(request):
    return render(request, 'home.html')

def student_form(request):
    student_details = None
    percentage = None

    if request.method == 'POST':
        form = StudentForm(request.POST)
        if form.is_valid():
            # Get form data
            name = form.cleaned_data['name']
            dob = form.cleaned_data['dob']
            address = form.cleaned_data['address']
            contact_number = form.cleaned_data['contact_number']
            email = form.cleaned_data['email']
            english_marks = form.cleaned_data['english_marks']
            physics_marks = form.cleaned_data['physics_marks']
            chemistry_marks = form.cleaned_data['chemistry_marks']

            # Calculate the total percentage
            total_marks = english_marks + physics_marks + chemistry_marks
            percentage = (total_marks / 300) * 100

            student_details = {
                'name': name,
                'dob': dob,
                'address': address,
                'contact_number': contact_number,
                'email': email,
                'english_marks': english_marks,
                'physics_marks': physics_marks,
                'chemistry_marks': chemistry_marks,
            }

    else:
        form = StudentForm()

    return render(request, 'student_form/student_form.html', {'form': form, 'student_details': student_details, 'percentage': percentage})
