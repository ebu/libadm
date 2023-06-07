#!/bin/bash

# updates requirements.txt by installing the latest requirements into a
# temporary virtualenv

REQUIREMENTS="breathe sphinx-rtd-theme m2r2"

###

set -eu

ENV=$(mktemp -d env.XXXXX)
python -m venv $ENV
source $ENV/bin/activate

pip install $REQUIREMENTS
pip freeze | grep -v '^-' > requirements.txt

deactivate
rm -r $ENV
