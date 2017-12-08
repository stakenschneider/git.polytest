import random
import string
import time

from datetime import datetime
from django.core.management.base import BaseCommand, CommandError
from DatabaseORM.models import *
from random import randint
from django.core.exceptions import ObjectDoesNotExist, ValidationError
from django.db import DatabaseError

FILE_NAME = 'DatabaseORM/data/'
FILE_EXTENSION = '.data'
DATETIME_FORMAT = '%d.%m.%Y %H:%M:%S'


class Command(BaseCommand):
    def add_arguments(self, parser):
        parser.add_argument('table_name', type=str)
        parser.add_argument('count', type=int)

    @staticmethod
    def generate_string(length):
        return ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(length))

    @staticmethod
    def generate_double_in_range(minimum, maximum):
        return random.uniform(minimum, maximum)

    @staticmethod
    def generate_integer_in_range(minimum, maximum):
        return randint(minimum, maximum)

    @staticmethod
    def generate_date_in_range(minimum, maximum):
        epoch = datetime.utcfromtimestamp(0)
        minimum_integer = (minimum - epoch).total_seconds()
        maximum_integer = (maximum - epoch).total_seconds()

        result_integer = Command.generate_integer_in_range(minimum_integer, maximum_integer)
        return datetime.fromtimestamp(result_integer)

    @staticmethod
    def generate_boolean():
        return bool(random.getrandbits(1))

    @staticmethod
    def generate_drug(count):
        try:
            id_drug = Drug.objects.latest('id_drug').id_drug
        except ObjectDoesNotExist:
            id_drug = -1

        drug_index = 0
        while drug_index < count:
            id_drug += 1

            drug_name = None
            contains = True
            while contains:
                drug_name = Command.generate_string(int(NAME_LENGTH / 16))
                contains = bool(Drug.objects.filter(drug_name__contains=drug_name).count() > 0)

            drug_shelf_life = Command.generate_integer_in_range(10, 80)
            drug_current_cost = Command.generate_double_in_range(10, 80)
            drug_recipe_needed = Command.generate_boolean()

            Drug(id_drug=id_drug, drug_name=drug_name, drug_shelf_life=drug_shelf_life, drug_current_cost=drug_current_cost, drug_recipe_needed=drug_recipe_needed).save()
            drug_index += 1

        Command.generate_disease(int(count / 2))

    @staticmethod
    def generate_disease(count):
        try:
            id_disease = Disease.objects.latest('id_disease').id_disease
        except ObjectDoesNotExist:
            id_disease = -1

        disease_index = 0
        while disease_index < count:
            id_disease += 1

            disease_name = None
            contains = True
            while contains:
                disease_name = Command.generate_string(int(NAME_LENGTH / 16))
                contains = bool(Disease.objects.filter(disease_name__contains=disease_name).count() > 0)

            Disease(id_disease=id_disease, disease_name=disease_name).save()
            disease_index += 1

        Command.generate_incompatibility(int(count / 4))

    @staticmethod
    def generate_incompatibility(count):
        try:
            id_incompatibility = Incompatibility.objects.latest('id_incompatibility').id_incompatibility
        except ObjectDoesNotExist:
            id_incompatibility = -1

        incompatibility_index = 0
        while incompatibility_index < count:
            id_incompatibility += 1

            id_drug_first = None
            id_drug_second = None

            contains = True
            max_id_drug = Drug.objects.latest('id_drug').id_drug
            while contains:
                id_drug_first = Command.generate_integer_in_range(0, max_id_drug)
                id_drug_second = Command.generate_integer_in_range(0, max_id_drug)

                if id_drug_first == id_drug_second:
                    continue

                contains = bool(Drug.objects.filter(id_drug__contains=id_drug_first).count() > 0)
                contains = contains and bool(Drug.objects.filter(id_drug__contains=id_drug_second).count() > 0)

                if not contains:
                    contains = True
                    continue
                try:
                    id_drug_first = Drug.objects.get(id_drug=id_drug_first)
                    id_drug_second = Drug.objects.get(id_drug=id_drug_second)
                except ObjectDoesNotExist:
                    continue

                contains = bool(Incompatibility.objects.filter(id_drug_first=id_drug_first).filter(id_drug_first=id_drug_second).count() > 0)
                contains = contains or bool(Incompatibility.objects.filter(id_drug_first=id_drug_second).filter(id_drug_first=id_drug_first).count() > 0)

            try:
                Incompatibility(id_incompatibility=id_incompatibility, id_drug_first=id_drug_first, id_drug_second=id_drug_second).save()
            except ObjectDoesNotExist:
                id_incompatibility -= 1
                continue

            incompatibility_index += 1

        Command.generate_indication(int(count))

    @staticmethod
    def generate_indication(count):
        try:
            id_indication = Indication.objects.latest('id_indication').id_indication
        except ObjectDoesNotExist:
            id_indication = -1

        indication_index = 0
        while indication_index < count:
            id_indication += 1

            id_disease = None
            id_drug = None

            contains = True
            max_id_disease = Disease.objects.latest('id_disease').id_disease
            max_id_drug = Drug.objects.latest('id_drug').id_drug
            while contains:
                id_disease = Command.generate_integer_in_range(0, max_id_disease)
                id_drug = Command.generate_integer_in_range(0, max_id_drug)

                contains = bool(Disease.objects.filter(id_disease__contains=id_disease).count() > 0)
                contains = contains and bool(Drug.objects.filter(id_drug__contains=id_drug).count() > 0)

                if not contains:
                    contains = True
                    continue

                try:
                    id_disease = Disease.objects.get(id_disease=id_disease)
                    id_drug = Drug.objects.get(id_drug=id_drug)
                except ObjectDoesNotExist:
                    continue

                contains = bool(Indication.objects.filter(id_disease=id_disease).filter(id_drug=id_drug).count() > 0)
                contains = contains or bool(Contraindication.objects.filter(id_disease=id_disease).filter(id_drug=id_drug).count() > 0)

            try:
                Indication(id_indication=id_indication, id_disease=id_disease, id_drug=id_drug).save()
            except ObjectDoesNotExist:
                id_indication -= 1
                continue

            indication_index += 1

        Command.generate_contraindication(int(count))

    @staticmethod
    def generate_contraindication(count):
        try:
            id_contraindication = Contraindication.objects.latest('id_contraindication').id_contraindication
        except ObjectDoesNotExist:
            id_contraindication = -1

        contraindication_index = 0
        while contraindication_index < count:
            id_contraindication += 1

            id_disease = None
            id_drug = None

            contains = True
            max_id_disease = Disease.objects.latest('id_disease').id_disease
            max_id_drug = Drug.objects.latest('id_drug').id_drug
            while contains:
                id_disease = Command.generate_integer_in_range(0, max_id_disease)
                id_drug = Command.generate_integer_in_range(0, max_id_drug)

                contains = bool(Disease.objects.filter(id_disease__contains=id_disease).count() > 0)
                contains = contains and bool(Drug.objects.filter(id_drug__contains=id_drug).count() > 0)

                if not contains:
                    contains = True
                    continue

                try:
                    id_disease = Disease.objects.get(id_disease=id_disease)
                    id_drug = Drug.objects.get(id_drug=id_drug)
                except ObjectDoesNotExist:
                    continue

                contains = bool(Indication.objects.filter(id_disease=id_disease).filter(id_drug=id_drug).count() > 0)
                contains = contains or bool(Contraindication.objects.filter(id_disease=id_disease).filter(id_drug=id_drug).count() > 0)

            try:
                Contraindication(id_contraindication=id_contraindication, id_disease=id_disease, id_drug=id_drug).save()
            except ObjectDoesNotExist:
                id_contraindication -= 1
                continue

            contraindication_index += 1

        Command.generate_provider(int(4 * count))

    @staticmethod
    def generate_provider(count):
        try:
            id_provider = Provider.objects.latest('id_provider').id_provider
        except ObjectDoesNotExist:
            id_provider = -1

        provider_index = 0
        while provider_index < count:
            id_provider += 1

            provider_name = None
            contains = True
            while contains:
                provider_name = Command.generate_string(int(NAME_LENGTH / 16))
                contains = bool(Provider.objects.filter(provider_name__contains=provider_name).count() > 0)

            Provider(id_provider=id_provider, provider_name=provider_name).save()
            provider_index += 1

        Command.generate_drugstore(int(count))

    @staticmethod
    def generate_drugstore(count):
        try:
            id_drugstore = Drugstore.objects.latest('id_drugstore').id_drugstore
        except ObjectDoesNotExist:
            id_drugstore = -1

        drugstore_index = 0
        while drugstore_index < count:
            id_drugstore += 1

            drugstore_address = None
            contains = True
            while contains:
                drugstore_address = Command.generate_string(int(ADDRESS_LENGTH / 16))
                contains = bool(Drugstore.objects.filter(drugstore_address__contains=drugstore_address).count() > 0)

            Drugstore(id_drugstore=id_drugstore, drugstore_address=drugstore_address).save()
            drugstore_index += 1

        Command.generate_consignment(int(4 * count))

    @staticmethod
    def generate_consignment(count):
        try:
            id_consignment = Consignment.objects.latest('id_consignment').id_consignment
        except ObjectDoesNotExist:
            id_consignment = -1

        consignment_index = 0
        while consignment_index < count:
            id_consignment += 1

            id_drug = None
            contains = True
            max_id_drug = Drug.objects.latest('id_drug').id_drug
            while contains:
                id_drug = Command.generate_integer_in_range(0, max_id_drug)
                contains = not bool(Drug.objects.filter(id_drug__contains=id_drug).count() > 0)
                if not contains:
                    try:
                        id_drug = Drug.objects.get(id_drug=id_drug)
                    except ObjectDoesNotExist:
                        contains = True
                        continue

            id_provider = None
            contains = True
            max_id_provider = Provider.objects.latest('id_provider').id_provider
            while contains:
                id_provider = Command.generate_integer_in_range(0, max_id_provider)
                contains = not bool(Provider.objects.filter(id_provider__contains=id_provider).count() > 0)
                if not contains:
                    try:
                        id_provider = Provider.objects.get(id_provider=id_provider)
                    except ObjectDoesNotExist:
                        contains = True
                        continue

            id_drugstore = None
            contains = True
            max_id_drugstore = Drugstore.objects.latest('id_drugstore').id_drugstore
            while contains:
                id_drugstore = Command.generate_integer_in_range(0, max_id_drugstore)
                contains = not bool(Drugstore.objects.filter(id_drugstore__contains=id_drugstore).count() > 0)
                if not contains:
                    try:
                        id_drugstore = Drugstore.objects.get(id_drugstore=id_drugstore)
                    except ObjectDoesNotExist:
                        contains = True
                        continue

            consignment_drug_count = Command.generate_integer_in_range(1000, 10000)

            minimum = datetime(2010, 1, 1)
            maximum = datetime(2015, 1, 1)
            while True:
                consignment_arrival_date = Command.generate_date_in_range(minimum, maximum)
                consignment_manufacture_date = Command.generate_date_in_range(minimum, maximum)

                if consignment_arrival_date >= consignment_manufacture_date:
                    break

            try:
                Consignment(id_consignment=id_consignment, id_drug=id_drug, id_provider=id_provider, id_drugstore=id_drugstore,
                        consignment_drug_count=consignment_drug_count, consignment_arrival_date=consignment_arrival_date,
                        consignment_manufacture_date=consignment_manufacture_date).save()
            except ObjectDoesNotExist:
                id_consignment -= 1
                continue

            consignment_index += 1

        Command.generate_client(int(count / 16))

    @staticmethod
    def generate_client(count):
        try:
            id_client = Client.objects.latest('id_client').id_client
        except ObjectDoesNotExist:
            id_client = -1

        client_index = 0
        while client_index < count:
            id_client += 1

            client_name = None
            contains = True
            while contains:
                client_name = Command.generate_string(int(NAME_LENGTH / 16))
                contains = bool(Client.objects.filter(client_name__contains=client_name).count() > 0)

            Client(id_client=id_client, client_name=client_name).save()
            client_index += 1

        Command.generate_request_part(int(2 * count))

    @staticmethod
    def generate_request_part(count):
        try:
            id_request_part = RequestPart.objects.latest('id_request_part').id_request_part
        except ObjectDoesNotExist:
            id_request_part = -1

        request_part_index = 0
        while request_part_index < count:
            id_request_part += 1

            id_client = None
            contains = True
            max_id_client = Client.objects.latest('id_client').id_client
            while contains:
                id_client = Command.generate_integer_in_range(0, max_id_client)
                contains = not bool(Client.objects.filter(id_client__contains=id_client).count() > 0)
                if not contains:
                    try:
                        id_client = Client.objects.get(id_client=id_client)
                    except ObjectDoesNotExist:
                        contains = True
                        continue

            request_part_date = Command.generate_date_in_range(datetime(2015, 1, 1), datetime(2017, 1, 1))

            try:
                RequestPart(id_request_part=id_request_part, id_client=id_client, request_part_date=request_part_date).save()
            except ObjectDoesNotExist:
                id_request_part -= 1
                continue

            request_part_index += 1

        Command.generate_request(int(4 * count))

    @staticmethod
    def generate_request(count):
        try:
            id_request = Request.objects.latest('id_request').id_request
        except ObjectDoesNotExist:
            id_request = -1

        request_index = 0
        while request_index < count:
            id_request += 1

            id_request_part = None
            contains = True
            max_id_request_part = RequestPart.objects.latest('id_request_part').id_request_part
            while contains:
                id_request_part = Command.generate_integer_in_range(0, max_id_request_part)
                contains = not bool(RequestPart.objects.filter(id_request_part__contains=id_request_part).count() > 0)
                if not contains:
                    try:
                        id_request_part = RequestPart.objects.get(id_request_part=id_request_part)
                    except ObjectDoesNotExist:
                        contains = True
                        continue

            id_consignment = None
            contains = True
            max_id_consignment = Consignment.objects.latest('id_consignment').id_consignment
            while contains:
                id_consignment = Command.generate_integer_in_range(0, max_id_consignment)
                contains = not bool(Consignment.objects.filter(id_consignment__contains=id_consignment).count() > 0)
                if not contains:
                    try:
                        id_consignment = Consignment.objects.get(id_consignment=id_consignment)
                    except ObjectDoesNotExist:
                        contains = True
                        continue

            request_drug_previous_cost = Command.generate_double_in_range(10, 80)
            request_count = Command.generate_integer_in_range(10, 100)

            try:
                Request(id_request=id_request, id_request_part=id_request_part, id_consignment=id_consignment,
                    request_drug_previous_cost=request_drug_previous_cost, request_count=request_count).save()
            except ObjectDoesNotExist:
                    id_request -= 1;
                    continue
                    
            request_index += 1

    def handle(self, *args, **options):
        table_name = options['table_name']
        count = int(options['count'])

        if count < 0:
            raise CommandError('Wrong count argument.')

        try:
            if table_name == 'disease':
                if count % 4 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(count * 2)
            elif table_name == 'drug':
                if count % 8 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(count)
            elif table_name == 'incompatibility':
                self.generate_drug(count * 8)
            elif table_name == 'indication':
                self.generate_drug(count * 8)
            elif table_name == 'contraindication':
                self.generate_drug(count * 8)
            elif table_name == 'provider':
                if count % 4 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(count * 2)
            elif table_name == 'drugstore':
                if count % 4 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(count * 2)
            elif table_name == 'consignment':
                if count % 16 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(int(count / 2))
            elif table_name == 'client':
                self.generate_drug(count * 8)
            elif table_name == 'request_part':
                if count % 2 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(count * 4)
            elif table_name == 'request':
                if count % 8 != 0:
                    raise CommandError('Wrong count argument.')
                self.generate_drug(count)
            else:
                raise CommandError('Wrong table name argument.')
        except (DatabaseError, ValueError, ValidationError) as exception:
            raise CommandError('Unknown exception.')
