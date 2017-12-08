# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.db import migrations
from datetime import datetime

APPLICATION_NAME = 'DatabaseORM'


def insert_data(apps, schema_editor):
    alias = schema_editor.connection.alias

    disease = apps.get_model(APPLICATION_NAME, 'Disease')
    disease.objects.using(alias).bulk_create(
        [
            disease(id_disease=1, disease_name='Ветрянка'),
            disease(id_disease=2, disease_name='Корь'),
            disease(id_disease=3, disease_name='ОРЗ'),
            disease(id_disease=4, disease_name='Язва'),
            disease(id_disease=5, disease_name='Импотенция'),
        ]
    )

    drug = apps.get_model(APPLICATION_NAME, 'Drug')
    drug.objects.using(alias).bulk_create(
            [
                drug(id_drug=1, drug_name='Аспирин', drug_shelf_life=30, drug_current_cost=32.22, drug_recipe_needed=False),
                drug(id_drug=2, drug_name='Нурафен', drug_shelf_life=40, drug_current_cost=55.10, drug_recipe_needed=False),
                drug(id_drug=3, drug_name='Аспоркам', drug_shelf_life=20, drug_current_cost=69.80, drug_recipe_needed=True),
                drug(id_drug=4, drug_name='Ношпа', drug_shelf_life=35, drug_current_cost=20.90, drug_recipe_needed=False),
                drug(id_drug=5, drug_name='Диазолин', drug_shelf_life=50, drug_current_cost=11.0, drug_recipe_needed=False),
                drug(id_drug=6, drug_name='Мезим', drug_shelf_life=30, drug_current_cost=41.20, drug_recipe_needed=False),
                drug(id_drug=7, drug_name='Регидрон', drug_shelf_life=70, drug_current_cost=5.40, drug_recipe_needed=False),
                drug(id_drug=8, drug_name='Флуоксетин', drug_shelf_life=50, drug_current_cost=15.40, drug_recipe_needed=True),
                drug(id_drug=9, drug_name='Супрадин', drug_shelf_life=30, drug_current_cost=25.0, drug_recipe_needed=False),
            ]
    )

    incompatibility = apps.get_model(APPLICATION_NAME, 'Incompatibility')
    incompatibility.objects.using(alias).bulk_create(
            [
                incompatibility(id_incompatibility=1, id_drug_first=drug.objects.get(id_drug=1), id_drug_second=drug.objects.get(id_drug=8)),
                incompatibility(id_incompatibility=2, id_drug_first=drug.objects.get(id_drug=2), id_drug_second=drug.objects.get(id_drug=3)),
                incompatibility(id_incompatibility=3, id_drug_first=drug.objects.get(id_drug=1), id_drug_second=drug.objects.get(id_drug=2)),
                incompatibility(id_incompatibility=4, id_drug_first=drug.objects.get(id_drug=4), id_drug_second=drug.objects.get(id_drug=3)),
                incompatibility(id_incompatibility=5, id_drug_first=drug.objects.get(id_drug=6), id_drug_second=drug.objects.get(id_drug=8)),
                incompatibility(id_incompatibility=6, id_drug_first=drug.objects.get(id_drug=3), id_drug_second=drug.objects.get(id_drug=7)),
                incompatibility(id_incompatibility=7, id_drug_first=drug.objects.get(id_drug=9), id_drug_second=drug.objects.get(id_drug=1)),
                incompatibility(id_incompatibility=8, id_drug_first=drug.objects.get(id_drug=3), id_drug_second=drug.objects.get(id_drug=4)),
            ]
    )

    indication = apps.get_model(APPLICATION_NAME, 'Indication')
    indication.objects.using(alias).bulk_create(
            [
                indication(id_indication=1, id_disease=disease.objects.get(id_disease=1), id_drug=drug.objects.get(id_drug=1)),
                indication(id_indication=2, id_disease=disease.objects.get(id_disease=2), id_drug=drug.objects.get(id_drug=2)),
                indication(id_indication=3, id_disease=disease.objects.get(id_disease=3), id_drug=drug.objects.get(id_drug=4)),
                indication(id_indication=4, id_disease=disease.objects.get(id_disease=4), id_drug=drug.objects.get(id_drug=4)),
                indication(id_indication=5, id_disease=disease.objects.get(id_disease=5), id_drug=drug.objects.get(id_drug=5)),
                indication(id_indication=6, id_disease=disease.objects.get(id_disease=5), id_drug=drug.objects.get(id_drug=7)),
                indication(id_indication=7, id_disease=disease.objects.get(id_disease=4), id_drug=drug.objects.get(id_drug=7)),
                indication(id_indication=8, id_disease=disease.objects.get(id_disease=4), id_drug=drug.objects.get(id_drug=9)),
                indication(id_indication=9, id_disease=disease.objects.get(id_disease=3), id_drug=drug.objects.get(id_drug=8)),
                indication(id_indication=10, id_disease=disease.objects.get(id_disease=3), id_drug=drug.objects.get(id_drug=5)),
                indication(id_indication=11, id_disease=disease.objects.get(id_disease=2), id_drug=drug.objects.get(id_drug=6)),
                indication(id_indication=12, id_disease=disease.objects.get(id_disease=1), id_drug=drug.objects.get(id_drug=5)),
            ]
    )

    contraindication = apps.get_model(APPLICATION_NAME, 'Contraindication')
    contraindication.objects.using(alias).bulk_create(
            [
                contraindication(id_contraindication=1, id_disease=disease.objects.get(id_disease=4), id_drug=drug.objects.get(id_drug=8)),
                contraindication(id_contraindication=2, id_disease=disease.objects.get(id_disease=4), id_drug=drug.objects.get(id_drug=3)),
                contraindication(id_contraindication=3, id_disease=disease.objects.get(id_disease=1), id_drug=drug.objects.get(id_drug=6)),
                contraindication(id_contraindication=4, id_disease=disease.objects.get(id_disease=1), id_drug=drug.objects.get(id_drug=2)),
                contraindication(id_contraindication=5, id_disease=disease.objects.get(id_disease=3), id_drug=drug.objects.get(id_drug=3)),
                contraindication(id_contraindication=6, id_disease=disease.objects.get(id_disease=3), id_drug=drug.objects.get(id_drug=7)),
                contraindication(id_contraindication=7, id_disease=disease.objects.get(id_disease=3), id_drug=drug.objects.get(id_drug=9)),
                contraindication(id_contraindication=8, id_disease=disease.objects.get(id_disease=4), id_drug=drug.objects.get(id_drug=2)),
                contraindication(id_contraindication=9, id_disease=disease.objects.get(id_disease=5), id_drug=drug.objects.get(id_drug=6)),
                contraindication(id_contraindication=10, id_disease=disease.objects.get(id_disease=5), id_drug=drug.objects.get(id_drug=1)),
                contraindication(id_contraindication=11, id_disease=disease.objects.get(id_disease=2), id_drug=drug.objects.get(id_drug=4)),
                contraindication(id_contraindication=12, id_disease=disease.objects.get(id_disease=2), id_drug=drug.objects.get(id_drug=5)),
            ]
    )

    provider = apps.get_model(APPLICATION_NAME, 'Provider')
    provider.objects.using(alias).bulk_create(
            [
                provider(id_provider=1, provider_name='Добрый доктор'),
                provider(id_provider=2, provider_name='Доставщики9000'),
                provider(id_provider=3, provider_name='Фармаштекер'),
                provider(id_provider=4, provider_name='Компания Бориса'),
                provider(id_provider=5, provider_name='Доктор силач'),
            ]
    )

    drugstore = apps.get_model(APPLICATION_NAME, 'Drugstore')
    drugstore.objects.using(alias).bulk_create(
            [
                drugstore(id_drugstore=1, drugstore_address='Москва, ул. Ленина 21к1'),
                drugstore(id_drugstore=2, drugstore_address='Санкт-Петербург, пр. Энгельса 10к2'),
                drugstore(id_drugstore=3, drugstore_address='Санкт-Петербург, пр. Невский 5к1'),
                drugstore(id_drugstore=4, drugstore_address='Санкт-Петербург, ул. Парашютная 12к1'),
            ]
    )

    consignment = apps.get_model(APPLICATION_NAME, 'Consignment')
    consignment.objects.using(alias).bulk_create(
            [
                consignment(id_consignment=1, id_drug=drug.objects.get(id_drug=5), id_provider=provider.objects.get(id_provider=3), id_drugstore=drugstore.objects.get(id_drugstore=1),
                            consignment_drug_count=1200, consignment_arrival_date=datetime(2011, 1, 10), consignment_manufacture_date=datetime(2010, 9, 25)),
                consignment(id_consignment=2, id_drug=drug.objects.get(id_drug=2), id_provider=provider.objects.get(id_provider=5), id_drugstore=drugstore.objects.get(id_drugstore=1),
                            consignment_drug_count=1000, consignment_arrival_date=datetime(2011, 2, 13), consignment_manufacture_date=datetime(2011, 2, 11)),
                consignment(id_consignment=3, id_drug=drug.objects.get(id_drug=7), id_provider=provider.objects.get(id_provider=4), id_drugstore=drugstore.objects.get(id_drugstore=4),
                            consignment_drug_count=3000, consignment_arrival_date=datetime(2011, 1, 14), consignment_manufacture_date=datetime(2011, 1, 10)),
                consignment(id_consignment=4, id_drug=drug.objects.get(id_drug=1), id_provider=provider.objects.get(id_provider=2), id_drugstore=drugstore.objects.get(id_drugstore=3),
                            consignment_drug_count=3000, consignment_arrival_date=datetime(2011, 2, 15), consignment_manufacture_date=datetime(2011, 2, 1)),
                consignment(id_consignment=5, id_drug=drug.objects.get(id_drug=3), id_provider=provider.objects.get(id_provider=1), id_drugstore=drugstore.objects.get(id_drugstore=3),
                            consignment_drug_count=6000, consignment_arrival_date=datetime(2011, 1, 10), consignment_manufacture_date=datetime(2010, 11, 11)),
                consignment(id_consignment=6, id_drug=drug.objects.get(id_drug=4), id_provider=provider.objects.get(id_provider=1), id_drugstore=drugstore.objects.get(id_drugstore=2),
                            consignment_drug_count=2000, consignment_arrival_date=datetime(2011, 1, 20), consignment_manufacture_date=datetime(2010, 12, 20)),
                consignment(id_consignment=7, id_drug=drug.objects.get(id_drug=5), id_provider=provider.objects.get(id_provider=5), id_drugstore=drugstore.objects.get(id_drugstore=2),
                            consignment_drug_count=1000, consignment_arrival_date=datetime(2011, 1, 24), consignment_manufacture_date=datetime(2011, 1, 21)),
                consignment(id_consignment=8, id_drug=drug.objects.get(id_drug=6), id_provider=provider.objects.get(id_provider=3), id_drugstore=drugstore.objects.get(id_drugstore=4),
                            consignment_drug_count=1000, consignment_arrival_date=datetime(2011, 2, 10), consignment_manufacture_date=datetime(2011, 2, 9)),
            ]
    )

    client = apps.get_model(APPLICATION_NAME, 'Client')
    client.objects.using(alias).bulk_create(
            [
                client(id_client=1, client_name='Валера'),
                client(id_client=2, client_name='Игнат'),
                client(id_client=3, client_name='Борис'),
            ]
    )

    request_part = apps.get_model(APPLICATION_NAME, 'RequestPart')
    request_part.objects.using(alias).bulk_create(
            [
                request_part(id_request_part=1, id_client=client.objects.get(id_client=1), request_part_date=datetime(2011, 3, 17)),
                request_part(id_request_part=2, id_client=client.objects.get(id_client=3), request_part_date=datetime(2011, 4, 2)),
                request_part(id_request_part=3, id_client=client.objects.get(id_client=2), request_part_date=datetime(2011, 3, 11)),
                request_part(id_request_part=4, id_client=client.objects.get(id_client=1), request_part_date=datetime(2011, 4, 22)),
                request_part(id_request_part=5, id_client=client.objects.get(id_client=1), request_part_date=datetime(2011, 4, 22)),
                request_part(id_request_part=6, id_client=client.objects.get(id_client=2), request_part_date=datetime(2011, 4, 28)),
                request_part(id_request_part=7, id_client=client.objects.get(id_client=3), request_part_date=datetime(2011, 4, 10)),
                request_part(id_request_part=8, id_client=client.objects.get(id_client=3), request_part_date=datetime(2011, 3, 2)),
            ]
    )

    request = apps.get_model(APPLICATION_NAME, 'Request')
    request.objects.using(alias).bulk_create(
            [
                request(id_request=1, id_request_part=request_part.objects.get(id_request_part=2), id_consignment=consignment.objects.get(id_consignment=1), request_drug_previous_cost=15.0, request_count=100),
                request(id_request=2, id_request_part=request_part.objects.get(id_request_part=3), id_consignment=consignment.objects.get(id_consignment=2), request_drug_previous_cost=25.0, request_count=50),
                request(id_request=3, id_request_part=request_part.objects.get(id_request_part=4), id_consignment=consignment.objects.get(id_consignment=3), request_drug_previous_cost=70.0, request_count=20),
                request(id_request=4, id_request_part=request_part.objects.get(id_request_part=1), id_consignment=consignment.objects.get(id_consignment=4), request_drug_previous_cost=15.0, request_count=200),
                request(id_request=5, id_request_part=request_part.objects.get(id_request_part=7), id_consignment=consignment.objects.get(id_consignment=5), request_drug_previous_cost=35.0, request_count=100),
                request(id_request=6, id_request_part=request_part.objects.get(id_request_part=8), id_consignment=consignment.objects.get(id_consignment=6), request_drug_previous_cost=10.0, request_count=400),
                request(id_request=7, id_request_part=request_part.objects.get(id_request_part=2), id_consignment=consignment.objects.get(id_consignment=7), request_drug_previous_cost=10.0, request_count=100),
                request(id_request=8, id_request_part=request_part.objects.get(id_request_part=3), id_consignment=consignment.objects.get(id_consignment=8), request_drug_previous_cost=15.0, request_count=50),
                request(id_request=9, id_request_part=request_part.objects.get(id_request_part=2), id_consignment=consignment.objects.get(id_consignment=1), request_drug_previous_cost=15.0, request_count=50),
                request(id_request=10, id_request_part=request_part.objects.get(id_request_part=1), id_consignment=consignment.objects.get(id_consignment=2), request_drug_previous_cost=25.0, request_count=150),
                request(id_request=11, id_request_part=request_part.objects.get(id_request_part=1), id_consignment=consignment.objects.get(id_consignment=3), request_drug_previous_cost=70.0, request_count=120),
                request(id_request=12, id_request_part=request_part.objects.get(id_request_part=7), id_consignment=consignment.objects.get(id_consignment=4), request_drug_previous_cost=15.0, request_count=250),
                request(id_request=13, id_request_part=request_part.objects.get(id_request_part=7), id_consignment=consignment.objects.get(id_consignment=5), request_drug_previous_cost=35.0, request_count=130),
                request(id_request=14, id_request_part=request_part.objects.get(id_request_part=6), id_consignment=consignment.objects.get(id_consignment=6), request_drug_previous_cost=10.0, request_count=100),
                request(id_request=15, id_request_part=request_part.objects.get(id_request_part=3), id_consignment=consignment.objects.get(id_consignment=7), request_drug_previous_cost=10.0, request_count=150),
                request(id_request=16, id_request_part=request_part.objects.get(id_request_part=8), id_consignment=consignment.objects.get(id_consignment=8), request_drug_previous_cost=15.0, request_count=250),
            ]
    )
    pass


def delete_data(apps, schema_editor):
    alias = schema_editor.connection.alias

    disease = apps.get_model(APPLICATION_NAME, 'Disease')
    disease.objects.using(alias).filter(id_disease__lte=5).delete()

    drug = apps.get_model(APPLICATION_NAME, 'Drug')
    drug.objects.using(alias).filter(id_drug__lte=9).delete()

    incompatibility = apps.get_model(APPLICATION_NAME, 'Incompatibility')
    incompatibility.objects.using(alias).filter(id_incompatibility__lte=8).delete()

    indication = apps.get_model(APPLICATION_NAME, 'Indication')
    indication.objects.using(alias).filter(id_indication__lte=12).delete()

    contraindication = apps.get_model(APPLICATION_NAME, 'Contraindication')
    contraindication.objects.using(alias).filter(id_contraindication__lte=12).delete()

    provider = apps.get_model(APPLICATION_NAME, 'Provider')
    provider.objects.using(alias).filter(id_provider__lte=5).delete()

    drugstore = apps.get_model(APPLICATION_NAME, 'Drugstore')
    drugstore.objects.using(alias).filter(id_drugstore__lte=4).delete()

    consignment = apps.get_model(APPLICATION_NAME, 'Consignment')
    consignment.objects.using(alias).filter(id_consignment__lte=8).delete()

    client = apps.get_model(APPLICATION_NAME, 'Client')
    client.objects.using(alias).filter(id_client__lte=3).delete()

    request_part = apps.get_model(APPLICATION_NAME, 'RequestPart')
    request_part.objects.using(alias).filter(id_request_part__lte=8).delete()

    request = apps.get_model(APPLICATION_NAME, 'Request')
    request.objects.using(alias).filter(id_request__lte=16).delete()
    pass


class Migration(migrations.Migration):

    dependencies = [
        (APPLICATION_NAME, '1_create_tables'),
    ]

    operations = [
        migrations.RunPython(insert_data, delete_data),
    ]
