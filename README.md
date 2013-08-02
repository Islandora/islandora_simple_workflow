BUILD STATUS
------------
Current build status:
[![Build Status](https://travis-ci.org/Islandora/islandora_simple_workflow.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_simple_workflow)

CI Server:
http://jenkins.discoverygarden.ca

ISLANDORA SIMPLE WORKFLOW
==================

A simple editorial workflow for Islandora. Ingested objects are inactive until approved.

When this module is enabled, ingested objects will have an inactive state. If a user has the permission
"Bypass default inactive object state" the default behaviour (object with active state) will persist. The
"Drupal Super User" (uid = 1) will always bypass the inactive state.
