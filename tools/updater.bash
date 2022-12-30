#!/bin/bash

dependency_client_update=$(<client/dependency_client_update.log)
dependency_model_generator_update=$(<data-management/tools/model-generator/dependency_model_generator_update.log)

git push --delete origin feature/full-npm-updates || echo "clean branches failed"

if [[ `git status --porcelain` ]]; then
  echo "=> Set configuration"
  git config user.name "MGuillaumeF"
  git config user.email "$EMAIL"
  echo "=> List of Updates :"
  git status
  git stash
  echo "=> Create Branch"
  git checkout -b feature/full-npm-updates
  git push -u origin feature/full-npm-updates
  echo "=> Index files"
  git stash pop
  git add client || echo "No modification to add pop"
  git add data-management/tools || echo "No modification to add pop"
  echo "=> Indexed Changes"
  git status
  echo "=> Save Changes"
  git commit -m "[AUTO][NPM_UPDATE] npm check updates

client : $dependency_client_update

model_generator : $dependency_model_generator_update"

  git push
  git switch main
  git merge --ff-only feature/full-npm-updates
  git push
  git push --delete origin feature/full-npm-updates || echo "clean branches failed"
fi
