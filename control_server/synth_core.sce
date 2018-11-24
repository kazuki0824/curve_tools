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
function [sys_c_p, sys_u_p, sys_u, sys_f ,E]=Motor(E,b,J,K,R,L,T_pwm)
    
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
//    Fpd=[Fp Fd]
    Fpd=[Fp Fd*0.05]
endfunction

function [ref, Idot]=calculate_ref(velo, accel, sys_c_p ,duty)
    [ref, Idot]=calculate_ref(velo, accel, sys_c_p ,duty, 0);
endfunction

function [ref, Idot]=calculate_ref(velo, accel, sys_c_p ,duty, Tf)
    V=duty*E;
    Mat = sys_c_p.A;
    intm = [-Mat(1,1)/Mat(1,2) 1/Mat(1,2) ; Mat(2,1) 0]*[velo;accel];
    ref=[velo;intm(1)];
    Idot=intm(2)-Mat(2,2)*intm(2,:)-sys_c_p.B(2,:)*V;
endfunction

function [Fpd,Kf,C1]=setenv_motor()
    E=24
    [sys_c_p, sys_u_p, sys_u, sys_f]=Motor(E, 0.01, 0.001020237876, 0.008, 0.485, 72.3/(10^6), 1/343/2)
    [Fpd, f_u, Ap, Bp]=getFpd(50,1)
    Kf=[-0.8484928;0.0139644;-0.0131325]
    [C1]=C1synth(Ap, Bp, sys_u, sys_f, Fpd, Kf)

    time=0:sys_u_p.dt:2
    plot2d(time',flts(ones(1,size(time)(2)),sys_u_p)')

endfunction

function simulation_triangle(duration , magn)
    E=24
    [Fpd,Kf,C1]=setenv_motor()
    C2=obscont(sys_f,Fpd,Kf)
    cl=sys_f/.-C2
    //plzr(cl*C1)
    
    //chirp signal
    f0= 0;//Hz
    f1=1;//Hz
    k=(f1-f0)/duration;
    
    //simulation
    X=[0;0;0];
    x_est=[0;0;0];
    xC1=[0;0;0;0]
    xC2=[0;0;0]
    u=0;
    
    realX_for_graph=[0;0;0];
    x_est_for_graph=[0;0;0];
    
    
    U1_for_graph=[0];
    U2_for_graph=[0];
    i=0
    tmp1=zeros(3,4);
    for time=0:sys_f.dt:duration
        //v_ref=cos(2*%pi*(f0*time+k*time*time/2)) * magn;
        v_ref=magn*(1-2*abs(round(time/4)-time/4));
        //a_ref=-sin(2*%pi*(f0*time+k*time*time/2))*k*time * magn;
        a_ref=sign(round(time/4)-time/4)*magn
        V=E*u;
        [ref2]=calculate_ref(v_ref, a_ref, sys_c_p ,u, x_est(3));
        ref1= (ref2+x_est(1:2,:))/2
        //C1
        [u1,xC1]=flts([ref1;ref2], C1, xC1);
        //C2
        u2=Fpd*x_est;
        
        U=u1+u2;
        for j=1:4
            if(U(j) < -1.0) then
                U(j) = -1.0;
            elseif (U(j) > 1.0) then
                U(j) = 1.0;
            end
        end
        
        u=(U(4)+U(3)+U(2)+U(1))/4;
        [X,tmp1]=ltitr(sys_u.A,sys_u.B,U',X)
        /**/
        realX_for_graph(1:3,4*i+1:4*(i+1))=tmp1;
        x_est_for_graph(1:3,1+(4*i):4*(i+1))=[x_est x_est x_est x_est ];
        U1_for_graph(1:1,1+(4*i):4*(i+1))=u1
        U2_for_graph(1:1,1+(4*i):4*(i+1))=u2
        /**/
                i=i+1;
        //Observer
        y=sys_u.C*X;
        x_est=sys_f.A*x_est+sys_f.B*U+Kf*(sys_f.C*x_est-y)
    end
    count= size(realX_for_graph)(2)
    t_axis=0:sys_f.dt/4:(sys_f.dt/4)*(count-1);
    //reference=cos(2*%pi*(f0*t_axis+k*(t_axis^2)/2))*magn;
    reference=magn*(1-2*abs(round(t_axis/4)-t_axis/4));
    scf();
    plot2d(t_axis, [realX_for_graph(1:3,:)'  reference(1,:)' ]);
    scf();
    plot2d(t_axis, [realX_for_graph(1,:)' x_est_for_graph(1,:)' ])
    scf();
    plot2d(t_axis, [realX_for_graph(2,:)' x_est_for_graph(2,:)' ])
    scf();
    plot2d(t_axis, [realX_for_graph(3,:)' x_est_for_graph(3,:)' ])
    scf();
    plot2d(t_axis, U1_for_graph')
    scf();
    plot2d(t_axis, U2_for_graph')
    scf();
    plot2d(t_axis, U1_for_graph'+U2_for_graph')
    
    //間引いて表示
    /*
    t_new=[0];x_new=[0;0;0];
    for i=1:size(t_axis)(2)
        if (modulo(i,2)==1) then
            t_new(:,ceil(i/2))= t_axis(:,i)
            x_new(:,ceil(i/2))= realX_for_graph(:,i)
        end
    end
    scf();
    plot2d(t_new, x_new')
    */
endfunction
