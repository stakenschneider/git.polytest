
...

INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'DatabaseORM',
]

...

DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.postgresql',
        'NAME': 'pharmacy',
        'USER': open('Configuration/user.txt', 'r').readline().rstrip(),
        'PASSWORD': open('Configuration/password.txt', 'r').readline().rstrip(),
        'HOST': '127.0.0.1',
        'PORT': '5432',
    }
}

...