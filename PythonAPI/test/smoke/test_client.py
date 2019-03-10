# Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma de
# Barcelona (UAB).
#
# This work is licensed under the terms of the MIT license.
# For a copy, see <https://opensource.org/licenses/MIT>.

import carla

import random
import re
import unittest

from . import TESTING_ADDRESS


class testClient(unittest.TestCase):
    def test_version(self):
        c = carla.Client(*TESTING_ADDRESS)
        self.assertEqual(c.get_client_version(), c.get_server_version())

    def test_reload_world(self):
        c = carla.Client(*TESTING_ADDRESS)
        map_name = c.get_world().get_map().name
        world = c.reload_world()
        self.assertEqual(map_name, world.get_map().name)

    def test_load_all_maps(self):
        c = carla.Client(*TESTING_ADDRESS)
        map_names = list(c.get_available_maps())
        random.shuffle(map_names)
        for map_name in map_names:
            world = c.load_world(map_name)
            self.assertEqual(map_name.split('/')[-1], world.get_map().name)


class testBlueprintLibrary(unittest.TestCase):
    def test_blueprint_ids(self):
        c = carla.Client(*TESTING_ADDRESS)
        library = c.get_world().get_blueprint_library()
        self.assertTrue([x for x in library])
        self.assertTrue([x for x in library.filter('sensor.*')])
        self.assertTrue([x for x in library.filter('static.*')])
        self.assertTrue([x for x in library.filter('vehicle.*')])
        self.assertTrue([x for x in library.filter('walker.*')])
        rgx = re.compile(r'\S+\.\S+\.\S+')
        for bp in library:
            self.assertTrue(rgx.match(bp.id))
        rgx = re.compile(r'(vehicle)\.\S+\.\S+')
        for bp in library.filter('vehicle.*'):
            self.assertTrue(rgx.match(bp.id))
