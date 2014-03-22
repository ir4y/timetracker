from flask import Flask
app = Flask(__name__)

app.config['SECRET_KEY'] = '123456790'
app.config['MONGODB_SETTINGS'] = {'DB': 'timetracker'}

if __name__ == '__main__':
    app.run(debug=True)
