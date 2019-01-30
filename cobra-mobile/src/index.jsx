import React from 'react';
import ReactDOM from 'react-dom';
import { BrowserRouter as Router, Route, Link, Switch, Redirect } from 'react-router-dom';

import './index.less';
import Login from './components/Login';
import Voucher from './components/Voucher'
import Register from './components/Register';
import Profile from './components/Profile';

class Index extends React.Component {
  render() {
    return (
      <div className="body">
      </div>
    );
  }
}

ReactDOM.render(
  <Router>
    <div>
      <Route path="/login" component={Login} />
      <Route path="/voucher" component={Voucher} />
      <Route path="/register" component={Register} />
      <Route path="/profile" component={Profile} />
      <Redirect from="/" to="/login"></Redirect>  
    </div>
  </Router>
  , document.getElementById('example'));
