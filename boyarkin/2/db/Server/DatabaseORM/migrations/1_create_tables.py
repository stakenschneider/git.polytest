# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = []

    operations = [
        migrations.CreateModel(
            name='Client',
            fields=[
                ('id_client', models.IntegerField(db_column='id_client', primary_key=True, serialize=False, unique=True)),
                ('client_name', models.CharField(db_column='client_name', max_length=256)),
            ],
            options={
                'db_table': 'client',
            },
        ),
        migrations.CreateModel(
            name='Consignment',
            fields=[
                ('id_consignment', models.IntegerField(db_column='id_consignment', primary_key=True, serialize=False, unique=True)),
                ('consignment_drug_count', models.IntegerField(db_column='consignment_drug_count')),
                ('consignment_arrival_date', models.DateField(db_column='consignment_arrival_date')),
                ('consignment_manufacture_date', models.DateField(db_column='consignment_manufacture_date')),
            ],
            options={
                'db_table': 'consignment',
            },
        ),
        migrations.CreateModel(
            name='Contraindication',
            fields=[
                ('id_contraindication', models.IntegerField(db_column='id_contraindication', primary_key=True, serialize=False, unique=True)),
            ],
            options={
                'db_table': 'contraindication',
            },
        ),
        migrations.CreateModel(
            name='Disease',
            fields=[
                ('id_disease', models.IntegerField(db_column='id_disease', primary_key=True, serialize=False, unique=True)),
                ('disease_name', models.CharField(db_column='disease_name', max_length=256, unique=True)),
            ],
            options={
                'db_table': 'disease',
            },
        ),
        migrations.CreateModel(
            name='Drug',
            fields=[
                ('id_drug', models.IntegerField(db_column='id_drug', primary_key=True, serialize=False, unique=True)),
                ('drug_name', models.CharField(db_column='drug_name', max_length=256, unique=True)),
                ('drug_shelf_life', models.IntegerField(db_column='drug_shelf_life')),
                ('drug_current_cost', models.DecimalField(db_column='drug_current_cost', decimal_places=4, max_digits=18)),
                ('drug_recipe_needed', models.BooleanField(db_column='drug_recipe_needed')),
            ],
            options={
                'db_table': 'drug',
            },
        ),
        migrations.CreateModel(
            name='Drugstore',
            fields=[
                ('id_drugstore', models.IntegerField(db_column='id_drugstore', primary_key=True, serialize=False, unique=True)),
                ('drugstore_address', models.CharField(db_column='drugstore_address', max_length=512, unique=True)),
            ],
            options={
                'db_table': 'drugstore',
            },
        ),
        migrations.CreateModel(
            name='Incompatibility',
            fields=[
                ('id_incompatibility', models.IntegerField(db_column='id_incompatibility', primary_key=True, serialize=False, unique=True)),
                ('id_drug_first', models.ForeignKey(db_column='id_drug_first', on_delete=django.db.models.deletion.CASCADE, related_name='id_drug_first', to='DatabaseORM.Drug')),
                ('id_drug_second', models.ForeignKey(db_column='id_drug_second', on_delete=django.db.models.deletion.CASCADE, related_name='id_drug_second', to='DatabaseORM.Drug')),
            ],
            options={
                'db_table': 'incompatibility',
            },
        ),
        migrations.CreateModel(
            name='Indication',
            fields=[
                ('id_indication', models.IntegerField(db_column='id_indication', primary_key=True, serialize=False, unique=True)),
                ('id_disease', models.ForeignKey(db_column='id_disease', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Disease')),
                ('id_drug', models.ForeignKey(db_column='id_drug', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Drug')),
            ],
            options={
                'db_table': 'indication',
            },
        ),
        migrations.CreateModel(
            name='Provider',
            fields=[
                ('id_provider', models.IntegerField(db_column='id_provider', primary_key=True, serialize=False, unique=True)),
                ('provider_name', models.CharField(db_column='provider_name', max_length=256, unique=True)),
            ],
            options={
                'db_table': 'provider',
            },
        ),
        migrations.CreateModel(
            name='Request',
            fields=[
                ('id_request', models.IntegerField(db_column='id_request', primary_key=True, serialize=False, unique=True)),
                ('request_drug_previous_cost', models.DecimalField(db_column='request_drug_previous_cost', decimal_places=4, max_digits=18)),
                ('request_count', models.IntegerField(db_column='request_count')),
                ('id_consignment', models.ForeignKey(db_column='id_consignment', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Consignment')),
            ],
            options={
                'db_table': 'request',
            },
        ),
        migrations.CreateModel(
            name='RequestPart',
            fields=[
                ('id_request_part', models.IntegerField(db_column='id_request_part', primary_key=True, serialize=False, unique=True)),
                ('request_part_date', models.DateField(auto_now_add=True, db_column='request_part_date')),
                ('id_client', models.ForeignKey(db_column='id_client', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Client')),
            ],
            options={
                'db_table': 'request_part',
            },
        ),
        migrations.AddField(
            model_name='request',
            name='id_request_part',
            field=models.ForeignKey(db_column='id_request_part', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.RequestPart'),
        ),
        migrations.AddField(
            model_name='contraindication',
            name='id_disease',
            field=models.ForeignKey(db_column='id_disease', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Disease'),
        ),
        migrations.AddField(
            model_name='contraindication',
            name='id_drug',
            field=models.ForeignKey(db_column='id_drug', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Drug'),
        ),
        migrations.AddField(
            model_name='consignment',
            name='id_drug',
            field=models.ForeignKey(db_column='id_drug', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Drug'),
        ),
        migrations.AddField(
            model_name='consignment',
            name='id_drugstore',
            field=models.ForeignKey(db_column='id_drugstore', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Drugstore'),
        ),
        migrations.AddField(
            model_name='consignment',
            name='id_provider',
            field=models.ForeignKey(db_column='id_provider', on_delete=django.db.models.deletion.CASCADE, to='DatabaseORM.Provider'),
        ),
    ]
