function res=statematch24(sys)
    [A,B,C,D] = abcd(sys);
    A_=A^4
    Mc =[  (A^3 * B) (A^2 * B) (A^1 * B) B ];
    B_ =Mc;
    C_ =C;
    res= syslin(sys.dt*4, A_,B_,C_);
endfunction

function [A_, B_]=intersample24(sys)
    A=sys.A; B=sys.B;
    A_=[A^2 ; A^4];
    O= zeros(3,1)
    B_1 = [A*B B O O]
    B_2 = [A^3*B A^2*B A*B B]
    B_=[B_1 ; B_2]
endfunction

/*************/
function [sys_c_p, sys_u_p, sys_u, sys_f]=Motor(E,b,J,K,R,L,T_pwm)
    
    A_cp=[-b/J K/J ; -K/L -R/L];
    B_cp=[0;1/L];
    C_cp=[1 0];
    sys_c_p=syslin('c',A_cp,B_cp,C_cp)
    Ap_s=expm(A_cp*T_pwm);
    Bp_s=expm(0.5*A_cp*T_pwm)*B_cp*E*T_pwm;
    Cp_s=C_cp;
    sys_u_p=syslin(T_pwm,Ap_s,Bp_s,Cp_s);

    A_torque = [1/J ; 0];
    alpha=0; //must be negative or 0
    A_c = [A_cp A_torque ; 0 0 alpha];
    B_c=[B_cp; 0];
    C_c=[C_cp 0];
    
    T_r = T_pwm *2
    T_y = T_r *2
    
    sys_u_p=syslin(T_pwm,Ap_s,Bp_s,Cp_s);
    A_s=expm(A_c*T_pwm);
    B_s=expm(0.5*A_c*T_pwm)*B_c*E*T_pwm;
    C_s=C_c;
    sys_u=syslin(T_pwm,A_s,B_s,C_s);
    ssprint(sys_u)
    ssprint(sys_u_p)
    //sys_f
    sys_f=statematch24(sys_u);
endfunction

/*************************************************************/

function [C1]=C1synth(Ap, Bp, sys_u, sys_f, Fpd, Kf)
    //C2 synthesis    
    C2=obscont(sys_f,Fpd,Kf);
    C2.dt=sys_f.dt;
    //C1 derivation
    z=poly(0,'z');
    z_=1/z
    F=-Bp\Ap;
    M=eye(4,4)+z_*F*sys_f.B(1:2,:);
    N=z_*sys_f.C(:,1:2)*sys_f.B(1:2,:);
    C1=minreal((M-C2*N)*(Bp^-1))
    C1.dt=C2.dt
endfunction

function [Fpd, f_u, Ap, Bp]=getFpd(current_weight, input_weight)
    //LQ
    f_u = lqr(sys_u_p,diag([0,current_weight]) , input_weight)
      
    [Ak , Bk]=intersample24(sys_u);
    
    //C2 anti-disturbance stabilization
    Ap=[Ak(1:2,1:2) ; Ak(4:5,1:2)]
    Bp=Bk;Bp(6,:)=[];Bp(3,:)=[];
    O=zeros(1,2);
    F_x=[f_u O ; f_u O ; O f_u ; O f_u];
    Fp=(eye(4,4)-F_x*Bp)\F_x*Ap;
    Apd=[Ak(1:2,3) ; Ak(4:5,3)];
    Fd = -Bp\Apd
    Fpd=[Fp Fd*0.05]
endfunction

function [ref, Idot]=calculate_ref(x, velo, accel, sys_c_p ,V)
    Mat = sys_c_p.A;
    intm = [-Mat(1,1)/Mat(1,2) 1/Mat(1,2) ; Mat(2,1) 0]*[velo;accel];
    ref=[v;intm(1)];
    Idot=intm(2)-Mat(2,2)*intm(2,:)-sys_c_p.B(2,:)*V;
endfunction
