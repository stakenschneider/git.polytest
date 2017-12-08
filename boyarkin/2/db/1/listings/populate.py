from django.core.management.base import BaseCommand
from DatabaseORM.models import *
from datetime import datetime


class Command(BaseCommand):

    @staticmethod
    def create_disease():
        Disease(id_disease=1, disease_name='Ветрянка').save()
        Disease(id_disease=2, disease_name='Корь').save()
        Disease(id_disease=3, disease_name='ОРЗ').save()
        Disease(id_disease=4, disease_name='Язва').save()
        Disease(id_disease=5, disease_name='Импотенция').save()

    @staticmethod
    def create_drug():
        Drug(id_drug=1, drug_name='Аспирин', drug_shelf_life=30, drug_current_cost=32.22, drug_recipe_needed=False).save()
        Drug(id_drug=2, drug_name='Нурафен', drug_shelf_life=40, drug_current_cost=55.10, drug_recipe_needed=False).save()
        Drug(id_drug=3, drug_name='Аспоркам', drug_shelf_life=20, drug_current_cost=69.80, drug_recipe_needed=True).save()
        Drug(id_drug=4, drug_name='Ношпа', drug_shelf_life=35, drug_current_cost=20.90, drug_recipe_needed=False).save()
        Drug(id_drug=5, drug_name='Диазолин', drug_shelf_life=50, drug_current_cost=11.0, drug_recipe_needed=False).save()
        Drug(id_drug=6, drug_name='Мезим', drug_shelf_life=30, drug_current_cost=41.20, drug_recipe_needed=False).save()
        Drug(id_drug=7, drug_name='Регидрон', drug_shelf_life=70, drug_current_cost=5.40, drug_recipe_needed=False).save()
        Drug(id_drug=8, drug_name='Флуоксетин', drug_shelf_life=50, drug_current_cost=15.40, drug_recipe_needed=True).save()
        Drug(id_drug=9, drug_name='Супрадин', drug_shelf_life=30, drug_current_cost=25.0, drug_recipe_needed=False).save()

    @staticmethod
    def create_incompatibility():
        Incompatibility(id_incompatibility=1, id_drug_first=Drug.objects.get(id_drug=1), id_drug_second=Drug.objects.get(id_drug=8)).save()
        Incompatibility(id_incompatibility=2, id_drug_first=Drug.objects.get(id_drug=2), id_drug_second=Drug.objects.get(id_drug=3)).save()
        Incompatibility(id_incompatibility=3, id_drug_first=Drug.objects.get(id_drug=1), id_drug_second=Drug.objects.get(id_drug=2)).save()
        Incompatibility(id_incompatibility=4, id_drug_first=Drug.objects.get(id_drug=4), id_drug_second=Drug.objects.get(id_drug=3)).save()
        Incompatibility(id_incompatibility=5, id_drug_first=Drug.objects.get(id_drug=6), id_drug_second=Drug.objects.get(id_drug=8)).save()
        Incompatibility(id_incompatibility=6, id_drug_first=Drug.objects.get(id_drug=3), id_drug_second=Drug.objects.get(id_drug=7)).save()
        Incompatibility(id_incompatibility=7, id_drug_first=Drug.objects.get(id_drug=9), id_drug_second=Drug.objects.get(id_drug=1)).save()
        Incompatibility(id_incompatibility=8, id_drug_first=Drug.objects.get(id_drug=3), id_drug_second=Drug.objects.get(id_drug=4)).save()

    @staticmethod
    def create_indication():
        Indication(id_indication=1, id_disease=Disease.objects.get(id_disease=1), id_drug=Drug.objects.get(id_drug=1)).save()
        Indication(id_indication=2, id_disease=Disease.objects.get(id_disease=2), id_drug=Drug.objects.get(id_drug=2)).save()
        Indication(id_indication=3, id_disease=Disease.objects.get(id_disease=3), id_drug=Drug.objects.get(id_drug=4)).save()
        Indication(id_indication=4, id_disease=Disease.objects.get(id_disease=4), id_drug=Drug.objects.get(id_drug=4)).save()
        Indication(id_indication=5, id_disease=Disease.objects.get(id_disease=5), id_drug=Drug.objects.get(id_drug=5)).save()
        Indication(id_indication=6, id_disease=Disease.objects.get(id_disease=5), id_drug=Drug.objects.get(id_drug=7)).save()
        Indication(id_indication=7, id_disease=Disease.objects.get(id_disease=4), id_drug=Drug.objects.get(id_drug=7)).save()
        Indication(id_indication=8, id_disease=Disease.objects.get(id_disease=4), id_drug=Drug.objects.get(id_drug=9)).save()
        Indication(id_indication=9, id_disease=Disease.objects.get(id_disease=3), id_drug=Drug.objects.get(id_drug=8)).save()
        Indication(id_indication=10, id_disease=Disease.objects.get(id_disease=3), id_drug=Drug.objects.get(id_drug=5)).save()
        Indication(id_indication=11, id_disease=Disease.objects.get(id_disease=2), id_drug=Drug.objects.get(id_drug=6)).save()
        Indication(id_indication=12, id_disease=Disease.objects.get(id_disease=1), id_drug=Drug.objects.get(id_drug=5)).save()

    @staticmethod
    def create_contraindication():
        Contraindication(id_contraindication=1, id_disease=Disease.objects.get(id_disease=4), id_drug=Drug.objects.get(id_drug=8)).save()
        Contraindication(id_contraindication=2, id_disease=Disease.objects.get(id_disease=4), id_drug=Drug.objects.get(id_drug=3)).save()
        Contraindication(id_contraindication=3, id_disease=Disease.objects.get(id_disease=1), id_drug=Drug.objects.get(id_drug=6)).save()
        Contraindication(id_contraindication=4, id_disease=Disease.objects.get(id_disease=1), id_drug=Drug.objects.get(id_drug=2)).save()
        Contraindication(id_contraindication=5, id_disease=Disease.objects.get(id_disease=3), id_drug=Drug.objects.get(id_drug=3)).save()
        Contraindication(id_contraindication=6, id_disease=Disease.objects.get(id_disease=3), id_drug=Drug.objects.get(id_drug=7)).save()
        Contraindication(id_contraindication=7, id_disease=Disease.objects.get(id_disease=3), id_drug=Drug.objects.get(id_drug=9)).save()
        Contraindication(id_contraindication=8, id_disease=Disease.objects.get(id_disease=4), id_drug=Drug.objects.get(id_drug=2)).save()
        Contraindication(id_contraindication=9, id_disease=Disease.objects.get(id_disease=5), id_drug=Drug.objects.get(id_drug=6)).save()
        Contraindication(id_contraindication=10, id_disease=Disease.objects.get(id_disease=5), id_drug=Drug.objects.get(id_drug=1)).save()
        Contraindication(id_contraindication=11, id_disease=Disease.objects.get(id_disease=2), id_drug=Drug.objects.get(id_drug=4)).save()
        Contraindication(id_contraindication=12, id_disease=Disease.objects.get(id_disease=2), id_drug=Drug.objects.get(id_drug=5)).save()

    @staticmethod
    def create_provider():
        Provider(id_provider=1, provider_name='Добрый доктор').save()
        Provider(id_provider=2, provider_name='Доставщики9000').save()
        Provider(id_provider=3, provider_name='Фармаштекер').save()
        Provider(id_provider=4, provider_name='Компания Бориса').save()
        Provider(id_provider=5, provider_name='Доктор силач').save()

    @staticmethod
    def create_drugstore():
        Drugstore(id_drugstore=1, drugstore_address='Москва, ул. Ленина 21к1').save()
        Drugstore(id_drugstore=2, drugstore_address='Санкт-Петербург, пр. Энгельса 10к2').save()
        Drugstore(id_drugstore=3, drugstore_address='Санкт-Петербург, пр. Невский 5к1').save()
        Drugstore(id_drugstore=4, drugstore_address='Санкт-Петербург, ул. Парашютная 12к1').save()

    @staticmethod
    def create_consignment():
        Consignment(id_consignment=1, id_drug=Drug.objects.get(id_drug=5), id_provider=Provider.objects.get(id_provider=3),
                    id_drugstore=Drugstore.objects.get(id_drugstore=1),
                    consignment_drug_count=1200, consignment_arrival_date=datetime(2011, 1, 10), consignment_manufacture_date=datetime(2010, 9, 25)).save()
        Consignment(id_consignment=2, id_drug=Drug.objects.get(id_drug=2), id_provider=Provider.objects.get(id_provider=5),
                    id_drugstore=Drugstore.objects.get(id_drugstore=1),
                    consignment_drug_count=1000, consignment_arrival_date=datetime(2011, 2, 13), consignment_manufacture_date=datetime(2011, 2, 11)).save()
        Consignment(id_consignment=3, id_drug=Drug.objects.get(id_drug=7), id_provider=Provider.objects.get(id_provider=4),
                    id_drugstore=Drugstore.objects.get(id_drugstore=4),
                    consignment_drug_count=3000, consignment_arrival_date=datetime(2011, 1, 14), consignment_manufacture_date=datetime(2011, 1, 10)).save()
        Consignment(id_consignment=4, id_drug=Drug.objects.get(id_drug=1), id_provider=Provider.objects.get(id_provider=2),
                    id_drugstore=Drugstore.objects.get(id_drugstore=3),
                    consignment_drug_count=3000, consignment_arrival_date=datetime(2011, 2, 15), consignment_manufacture_date=datetime(2011, 2, 1)).save()
        Consignment(id_consignment=5, id_drug=Drug.objects.get(id_drug=3), id_provider=Provider.objects.get(id_provider=1),
                    id_drugstore=Drugstore.objects.get(id_drugstore=3),
                    consignment_drug_count=6000, consignment_arrival_date=datetime(2011, 1, 10), consignment_manufacture_date=datetime(2010, 11, 11)).save()
        Consignment(id_consignment=6, id_drug=Drug.objects.get(id_drug=4), id_provider=Provider.objects.get(id_provider=1),
                    id_drugstore=Drugstore.objects.get(id_drugstore=2),
                    consignment_drug_count=2000, consignment_arrival_date=datetime(2011, 1, 20), consignment_manufacture_date=datetime(2010, 12, 20)).save()
        Consignment(id_consignment=7, id_drug=Drug.objects.get(id_drug=5), id_provider=Provider.objects.get(id_provider=5),
                    id_drugstore=Drugstore.objects.get(id_drugstore=2),
                    consignment_drug_count=1000, consignment_arrival_date=datetime(2011, 1, 24), consignment_manufacture_date=datetime(2011, 1, 21)).save()
        Consignment(id_consignment=8, id_drug=Drug.objects.get(id_drug=6), id_provider=Provider.objects.get(id_provider=3),
                    id_drugstore=Drugstore.objects.get(id_drugstore=4),
                    consignment_drug_count=1000, consignment_arrival_date=datetime(2011, 2, 10), consignment_manufacture_date=datetime(2011, 2, 9)).save()

    @staticmethod
    def create_client():
        Client(id_client=1, client_name='Валера').save()
        Client(id_client=2, client_name='Игнат').save()
        Client(id_client=3, client_name='Борис').save()

    @staticmethod
    def create_request_part():
        RequestPart(id_request_part=1, id_client=Client.objects.get(id_client=1), request_part_date=datetime(2011, 3, 17)).save()
        RequestPart(id_request_part=2, id_client=Client.objects.get(id_client=3), request_part_date=datetime(2011, 4, 2)).save()
        RequestPart(id_request_part=3, id_client=Client.objects.get(id_client=2), request_part_date=datetime(2011, 3, 11)).save()
        RequestPart(id_request_part=4, id_client=Client.objects.get(id_client=1), request_part_date=datetime(2011, 4, 22)).save()
        RequestPart(id_request_part=5, id_client=Client.objects.get(id_client=1), request_part_date=datetime(2011, 4, 22)).save()
        RequestPart(id_request_part=6, id_client=Client.objects.get(id_client=2), request_part_date=datetime(2011, 4, 28)).save()
        RequestPart(id_request_part=7, id_client=Client.objects.get(id_client=3), request_part_date=datetime(2011, 4, 10)).save()
        RequestPart(id_request_part=8, id_client=Client.objects.get(id_client=3), request_part_date=datetime(2011, 3, 2)).save()

    @staticmethod
    def create_request():
        Request(id_request=1, id_request_part=RequestPart.objects.get(id_request_part=2), id_consignment=Consignment.objects.get(id_consignment=1),
                request_drug_previous_cost=15.0, request_count=100).save()
        Request(id_request=2, id_request_part=RequestPart.objects.get(id_request_part=3), id_consignment=Consignment.objects.get(id_consignment=2),
                request_drug_previous_cost=25.0, request_count=50).save()
        Request(id_request=3, id_request_part=RequestPart.objects.get(id_request_part=4), id_consignment=Consignment.objects.get(id_consignment=3),
                request_drug_previous_cost=70.0, request_count=20).save()
        Request(id_request=4, id_request_part=RequestPart.objects.get(id_request_part=1), id_consignment=Consignment.objects.get(id_consignment=4),
                request_drug_previous_cost=15.0, request_count=200).save()
        Request(id_request=5, id_request_part=RequestPart.objects.get(id_request_part=7), id_consignment=Consignment.objects.get(id_consignment=5),
                request_drug_previous_cost=35.0, request_count=100).save()
        Request(id_request=6, id_request_part=RequestPart.objects.get(id_request_part=8), id_consignment=Consignment.objects.get(id_consignment=6),
                request_drug_previous_cost=10.0, request_count=400).save()
        Request(id_request=7, id_request_part=RequestPart.objects.get(id_request_part=2), id_consignment=Consignment.objects.get(id_consignment=7),
                request_drug_previous_cost=10.0, request_count=100).save()
        Request(id_request=8, id_request_part=RequestPart.objects.get(id_request_part=3), id_consignment=Consignment.objects.get(id_consignment=8),
                request_drug_previous_cost=15.0, request_count=50).save()
        Request(id_request=9, id_request_part=RequestPart.objects.get(id_request_part=2), id_consignment=Consignment.objects.get(id_consignment=1),
                request_drug_previous_cost=15.0, request_count=50).save()
        Request(id_request=10, id_request_part=RequestPart.objects.get(id_request_part=1), id_consignment=Consignment.objects.get(id_consignment=2),
                request_drug_previous_cost=25.0, request_count=150).save()
        Request(id_request=11, id_request_part=RequestPart.objects.get(id_request_part=1), id_consignment=Consignment.objects.get(id_consignment=3),
                request_drug_previous_cost=70.0, request_count=120).save()
        Request(id_request=12, id_request_part=RequestPart.objects.get(id_request_part=7), id_consignment=Consignment.objects.get(id_consignment=4),
                request_drug_previous_cost=15.0, request_count=250).save()
        Request(id_request=13, id_request_part=RequestPart.objects.get(id_request_part=7), id_consignment=Consignment.objects.get(id_consignment=5),
                request_drug_previous_cost=35.0, request_count=130).save()
        Request(id_request=14, id_request_part=RequestPart.objects.get(id_request_part=6), id_consignment=Consignment.objects.get(id_consignment=6),
                request_drug_previous_cost=10.0, request_count=100).save()
        Request(id_request=15, id_request_part=RequestPart.objects.get(id_request_part=3), id_consignment=Consignment.objects.get(id_consignment=7),
                request_drug_previous_cost=10.0, request_count=150).save()
        Request(id_request=16, id_request_part=RequestPart.objects.get(id_request_part=8), id_consignment=Consignment.objects.get(id_consignment=8),
                request_drug_previous_cost=15.0, request_count=250).save()

    def handle(self, *args, **options):
        self.create_disease()
        self.create_drug()
        self.create_incompatibility()
        self.create_indication()
        self.create_contraindication()
        self.create_provider()
        self.create_drugstore()
        self.create_consignment()
        self.create_client()
        self.create_request_part()
        self.create_request()
