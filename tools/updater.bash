#!/bin/bash

dependency_client_update=$(<client/dependency_client_update.log)
dependency_model_generator_update=$(<data-management/tools/model-generator/dependency_model_generator_update.log)

if [[ `git status --porcelain` ]]; then
  git stash
  git checkout -b feature/full-npm-updates
  git push -u origin feature/full-npm-updates
  git config user.name "MGuillaumeF"
  git config user.email "$EMAIL"
  git add client || echo "No modification to add pop"
  git add data-management/tools || echo "No modification to add pop"
  git commit -m "[AUTO][NPM_UPDATE] npm check update\nclient : $dependency_client_update\nmodel_generator : $dependency_model_generator_update" 
  git push
  git switch main
  git merge --ff-only feature/full-npm-updates
  git push
  git branch -d feature/full-npm-updates
fi
