git config user.name "circleci"
git config user.email "circleci@circleci.com"
git add .
git commit -m "Generate documentation $CIRCLE_BUILD_NUM"
git push origin