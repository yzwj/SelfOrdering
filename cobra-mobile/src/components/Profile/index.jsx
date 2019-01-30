import React from 'react'
import ReactDOM from 'react-dom'
import {  reqMember } from '../../api/requrl'
import { createForm } from 'rc-form';
import { Link } from 'react-router-dom'
import Cookies from 'js-cookie';
import voucher_logo from '../../assets/img/coupon.png'
import { Card, Flex, List, InputItem, Toast, Button, WingBlank, WhiteSpace, NavBar, Icon, Grid } from 'antd-mobile';

export default class Demo extends React.Component {
    state = {
      visible: false,
      confirmLoading: false,
      invicode: [],
      outlets: [],
      membership: [],
      voucherdata: [],
      topping: [],
      initializing: 1,
      hasMore: 0
    }
  
    async componentWillMount() {
      let { membership } = this.state
      this.setState({
        hasMore: 1,
        initializing: 2, // initialized
      });
      const Token = 'Token ' + Cookies.get('Authorization')
      let req = await reqMember(Token)
      membership = req.data.results
      console.log(membership[0].mobile_no)
      this.setState({ membership })
    }
  
    render() {
      let { membership } = this.state
      let now = new Date();
      let user = Cookies.get('user')
      const data = Array.from(new Array(1)).map((_val, i) => ({
        icon: voucher_logo,
        text: `Voucher`,
      }));
      return (
        Cookies.get('isLogin') ? (
        <div>
          <NavBar
            mode="light"
            icon={<Icon type="left" />}
            onLeftClick={() => this.props.history.goBack()}
            rightContent={[
              <Icon key="1" type="ellipsis" />,
            ]}
          >Profile</NavBar>
          {membership.map((member => {
            return(            
            <div>
              <WingBlank size="lg">
              <WhiteSpace size="lg" />
              <Card full>
                <Card.Header
                  title={member.mobile_no}
                  thumb="https://gw.alipayobjects.com/zos/rmsportal/MRhHctKOineMbKAZslML.jpg"
                  extra={<span>{member.total_points}</span>}
                />
                <Card.Body>
                  <div>{member.membership.name}</div>
                </Card.Body>
                <Card.Footer content="other things" extra={<div>something else</div>} />
              </Card>
              <WhiteSpace size="lg" />
              </WingBlank>
            </div>)}
            ))}
            <Grid data={data} activeStyle={true} columnNum={3} onClick={_el => this.props.history.push("/voucher")}/>
        </div>) : this.props.history.push("/login") );
    }
  }
  