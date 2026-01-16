@echo off

:execute
:: run the main cli script
python %~dp0modules.py %*
python %~dp0run.py %*