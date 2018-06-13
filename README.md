# Islandora Simple Workflow [![Build Status](https://travis-ci.org/Islandora/islandora_simple_workflow.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_simple_workflow)

## Introduction

A simple editorial workflow for Islandora. Ingested objects are inactive until approved.

When this module is enabled, ingested objects will be given the Fedora state 'inactive'. If the user creating the objects has the
permission "Bypass default inactive object state", objects will be given the normal, 'active' state. The "Drupal Super
User" (uid = 1) will always bypass the inactive state.

By default, 'inactive' objects are not displayed in collections and are not indexed in Solr, but they are accessible
directly (if the PID is known) by anyone who can view Islandora objects. To restrict access to inactive objects, check
the "Lock down inactive and deleted objects" option in Islandora configuration (admin/islandora/configure) and grant the
resulting permission to appropriate roles.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Inactive objects can be managed at Administration » Islandora » Simple Workflow objects (admin/islandora/tools/simple_workflow/list).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Simple+Workflow).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jordan Dukart](https://github.com/jordandukart)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
