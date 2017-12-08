from django.db import models

# Create your models here.

DECIMAL_MAX_LENGTH = 18
DECIMAL_PLACES = 4
NAME_LENGTH = 256
ADDRESS_LENGTH = 512


class Disease(models.Model):
    id_disease = models.IntegerField(db_column='id_disease', primary_key=True, unique=True, null=False)
    disease_name = models.CharField(db_column='disease_name', max_length=NAME_LENGTH, unique=True, null=False)

    class Meta:
        db_table = 'disease'


class Drug(models.Model):
    id_drug = models.IntegerField(db_column='id_drug', primary_key=True, unique=True, null=False)
    drug_name = models.CharField(db_column='drug_name', max_length=NAME_LENGTH, unique=True, null=False)
    drug_shelf_life = models.IntegerField(db_column='drug_shelf_life', unique=False, null=False)
    drug_current_cost = models.DecimalField(db_column='drug_current_cost', max_digits=DECIMAL_MAX_LENGTH, decimal_places=DECIMAL_PLACES, unique=False, null=False)
    drug_recipe_needed = models.BooleanField(db_column='drug_recipe_needed', unique=False, null=False)

    class Meta:
        db_table = 'drug'


class Indication(models.Model):
    id_indication = models.IntegerField(db_column='id_indication', primary_key=True, unique=True, null=False)
    id_disease = models.ForeignKey(Disease, db_column='id_disease', unique=False, null=False)
    id_drug = models.ForeignKey(Drug, db_column='id_drug', unique=False, null=False)

    class Meta:
        db_table = 'indication'


class Contraindication(models.Model):
    id_contraindication = models.IntegerField(db_column='id_contraindication', primary_key=True, unique=True, null=False)
    id_disease = models.ForeignKey(Disease, db_column='id_disease', unique=False, null=False)
    id_drug = models.ForeignKey(Drug, db_column='id_drug', unique=False, null=False)

    class Meta:
        db_table = 'contraindication'


class Incompatibility(models.Model):
    id_incompatibility = models.IntegerField(db_column='id_incompatibility', primary_key=True, unique=True, null=False)
    id_drug_first = models.ForeignKey(Drug, db_column='id_drug_first', related_name='id_drug_first', unique=False, null=False)
    id_drug_second = models.ForeignKey(Drug, db_column='id_drug_second', related_name='id_drug_second', unique=False, null=False)

    class Meta:
        db_table = 'incompatibility'


class Provider(models.Model):
    id_provider = models.IntegerField(db_column='id_provider', primary_key=True, unique=True, null=False)
    provider_name = models.CharField(db_column='provider_name', max_length=NAME_LENGTH, unique=True, null=False)

    class Meta:
        db_table = 'provider'


class Drugstore(models.Model):
    id_drugstore = models.IntegerField(db_column='id_drugstore', primary_key=True, unique=True, null=False)
    drugstore_address = models.CharField(db_column='drugstore_address', max_length=ADDRESS_LENGTH, unique=True, null=False)

    class Meta:
        db_table = 'drugstore'


class Client(models.Model):
    id_client = models.IntegerField(db_column='id_client', primary_key=True, unique=True, null=False)
    client_name = models.CharField(db_column='client_name', max_length=NAME_LENGTH, unique=False, null=False)

    class Meta:
        db_table = 'client'


class RequestPart(models.Model):
    id_request_part = models.IntegerField(db_column='id_request_part', primary_key=True, unique=True, null=False)
    id_client = models.ForeignKey(Client, db_column='id_client', unique=False, null=False)
    request_part_date = models.DateField(db_column='request_part_date', auto_now=False, auto_now_add=False, unique=False, null=False)

    class Meta:
        db_table = 'request_part'


class Consignment(models.Model):
    id_consignment = models.IntegerField(db_column='id_consignment', primary_key=True, unique=True, null=False)
    id_drug = models.ForeignKey(Drug, db_column='id_drug', unique=False, null=False)
    id_provider = models.ForeignKey(Provider, db_column='id_provider', unique=False, null=False)
    id_drugstore = models.ForeignKey(Drugstore, db_column='id_drugstore', unique=False, null=False)
    consignment_drug_count = models.IntegerField(db_column='consignment_drug_count', unique=False, null=False)
    consignment_arrival_date = models.DateField(db_column='consignment_arrival_date', auto_now=False, auto_now_add=False, unique=False, null=False)
    consignment_manufacture_date = models.DateField(db_column='consignment_manufacture_date', auto_now=False, auto_now_add=False, unique=False, null=False)

    class Meta:
        db_table = 'consignment'


class Request(models.Model):
    id_request = models.IntegerField(db_column='id_request', primary_key=True, unique=True, null=False)
    id_request_part = models.ForeignKey(RequestPart, db_column='id_request_part', unique=False, null=False)
    id_consignment = models.ForeignKey(Consignment, db_column='id_consignment', unique=False, null=False)
    request_drug_previous_cost = models.DecimalField(db_column='request_drug_previous_cost', max_digits=DECIMAL_MAX_LENGTH, decimal_places=DECIMAL_PLACES, unique=False, null=False)
    request_count = models.IntegerField(db_column='request_count', unique=False, null=False)

    class Meta:
        db_table = 'request'
