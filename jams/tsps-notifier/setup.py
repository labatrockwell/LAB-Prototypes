"""
Script for building the example.

Usage:
    python setup.py py2app
"""
from setuptools import setup

setup(
    app=["tspsExample.py"],
    data_files=["web"],
    setup_requires=["py2app"],
)
