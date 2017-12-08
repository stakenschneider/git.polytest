from django.contrib import admin
from DatabaseORM.models import *

# Register your models here.

admin.site.register([Disease, Drug, Indication, Contraindication, Incompatibility, Provider, Drugstore, Client, RequestPart, Consignment, Request])
