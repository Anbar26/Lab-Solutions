from django.urls import path  # Import path
from . import views

urlpatterns = [
    #path('', views.index, name='index'),
    path('', views.student_form, name='student_form'),  # Use path() instead of url()
]
